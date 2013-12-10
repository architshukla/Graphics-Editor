#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<GL/glut.h>

#include "Boxes.h"
#include "colorPalette.h"
#include "menuBar.h"

#define BRUSHSIZEINDEX 0
#define LINESIZEINDEX 1
#define PI 3.1415

#define NEW 100
#define OPEN 101
#define SAVE 102
#define EXIT 103

#define PENCIL 200
#define BRUSH 201
#define SPRAY 202
#define ERAZER 203
#define LINE 204
#define RECTANGLE 205
#define FILLEDRECTANGLE 206
#define TRIANGLE 207
#define FILLEDTRIANGLE 208
#define CIRCLE 209
#define FILLEDCIRCLE 210
#define SPHERE 211
#define FILLEDSPHERE 212
#define CUBE 213
#define FILLEDCUBE 214

#define SPIRAL 300
#define LIMACON 301
#define CARDIOD 302
#define THREELEAF 303

#define INNERCLIP 304
#define OUTERCLIP 305
#define SCALE 306
#define TRANSLATE 307
#define ROTATE 308
#define REFLECT 309
#define SHEAR 310
#define FLOODFILL 311

GLint XMAX = 1018;
GLint YMAX = 700;

GLint canvasXMIN = 1018 * 0.2;
GLint canvasYMIN = 700 * 0.1;
GLint canvasXMAX = 1018 * 0.95;
GLint canvasYMAX = 700 * 0.95;

GLint size[2] = {1, 1};
GLboolean sizeIndex = 0;

GLfloat colors[20][3]={	{0.0,0.0,0.0},		// Black
						{1.0,1.0,1.0},		// White
						{0.2,0.2,0.2},		// Dark Gray
						{0.7,0.7,0.7},		// Gray
						{0.5,0.0,0.0},		// Maroon
						{0.6,0.2,0.0},		// Brown
						{1.0,0.0,0.0},		// Red
						{1.0,0.6,0.8},		// Pink
						{1.0,0.26,0.0},		// Orange
						{1.0,0.6,0.0},		// Mustard
						{1.0,1.0,0.0},		// Yellow
						{1.0,1.0,0.8},		// OffWhite
						{0.0,0.6,0.2},		// Dark Green
						{0.6,1.0,0.2},		// Lime
						{0.0,0.6,1.0},		// Blue
						{0.4,1.0,1.0},		// Aqua
						{0.0,0.0,1.0},		// Dark Blue
						{0.4,0.6,0.6},		// Steel Blue
						{0.4,0.0,0.4},		// Purple
						{0.8,0.6,1.0} }; 	// Mauve
GLint clr = 0;
GLint padding = 5;
GLint optionHeight = 30;
GLint operation;
GLint oldx, oldy, oldxMax, oldyMax;
GLfloat matrix[2000][2000][3] = {0};
GLfloat clipmatrix[500][500][3] = {0};
long matrixSize = 2000 * 2000 * 3 * sizeof(long);
GLint scaleDone = 0, translateDone = 0, shearDone = 0, rotateDone = 0, reflectDone = 0;
float rotationAngle = 30.0 * PI / 180.0;
float reflectIntercept = 0, reflectM = 1; 
GLint shearX = 1, shearY = 0;

extern int numberOfColorColumns, numberOfMenuItems, numberOfLeftOptionItems, numberOfRightOptionItems;

int inside_area(float left, float right, float bottom, float top, float x, float y)
{
	if(x>left && x<right)
		if(y<top && y>bottom)
			return 1;
	return 0;
}

/*
	Drawing primitives
*/

void drawPoint(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void drawLine(float x0, float y0, float x1, float y1)
{
	glBegin(GL_LINES);
	glVertex2f(x0, y0);
	glVertex2f(x1, y1);
	glEnd();
	glFlush();
}

void drawTriangle(int topx, int topy, int lowerx, int lowery)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(topx, topy);
	glVertex2f(lowerx, lowery);
	glVertex2f(topx, lowery);
	glEnd();
	glFlush();
}

void drawFilledTriangle(int topx, int topy, int lowerx, int lowery)
{
	glBegin(GL_POLYGON);
	glVertex2f(topx, topy);
	glVertex2f(lowerx, lowery);
	glVertex2f(topx, lowery);
	glEnd();
	glFlush();
}

void drawRectangle(int minx, int miny, int maxx, int maxy)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(minx, miny);
	glVertex2f(minx, maxy);
	glVertex2f(maxx, maxy);
	glVertex2f(maxx, miny);
	glEnd();
	glFlush();
}

void drawFilledRectangle(int minx, int miny, int maxx, int maxy)
{
	glBegin(GL_POLYGON);
	glVertex2f(minx, miny);
	glVertex2f(minx, maxy);
	glVertex2f(maxx, maxy);
	glVertex2f(maxx, miny);
	glEnd();
	glFlush();
}

void plotSymmetricPoints(int h,int k,int x,int y)
{
	drawPoint(x + h,y + k);
	drawPoint(-x + h,y + k);
	drawPoint(x + h,-y + k);
	drawPoint(-x + h,-y + k);
	drawPoint(y + h,x + k);
	drawPoint(-y + h,x + k);
	drawPoint(y + h,-x + k);
	drawPoint(-y + h,-x + k);
}

void drawCircle(int xc, int yc, int r)
{
	if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, xc - r, yc))
		if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, xc + r, yc))
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, xc, yc + r))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, xc, yc - r))
				{

					glPointSize(1);
					int d = 1 - r, x = 0, y = r;
					while(y > x)
					{
						plotSymmetricPoints(xc, yc, x, y);
						if(d < 0)	d += 2*x+3;
						else
						{
							d += 2 * (x - y) + 5;
							y--;
						}
						x++;
					}
					plotSymmetricPoints(xc, yc, x, y);
					glFlush();
					glPointSize(size[BRUSHSIZEINDEX]);
				}
}

void drawFilledCircle(int xc, int yc, int r)
{
	if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, xc - r, yc))
		if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, xc + r, yc))
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, xc, yc + r))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, xc, yc - r))
				{
					int theta;
					float conversionFactor = PI / 180.0;
					glBegin(GL_POLYGON);
					for(theta = 0; theta < 360; theta++)
					{
						glVertex2f(xc + r * cosf(conversionFactor * theta), yc + r * sinf(conversionFactor * theta)) ;
					}
					glEnd();
					glFlush();
				}
}

void pencilStroke(int x, int y)
{
	if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x - size[BRUSHSIZEINDEX], y - size[BRUSHSIZEINDEX]))
		if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + size[BRUSHSIZEINDEX], y + size[BRUSHSIZEINDEX]))
		{
			glBegin(GL_POLYGON);
			glVertex2f(x + size[BRUSHSIZEINDEX], y);
			glVertex2f(x, y + size[BRUSHSIZEINDEX]);
			glVertex2f(x - size[BRUSHSIZEINDEX], y);
			glVertex2f(x, y - size[BRUSHSIZEINDEX]);
			glEnd();
		}
}

void brushStroke(int x, int y)
{
	int sizeOfBrush = size[BRUSHSIZEINDEX];
	size[BRUSHSIZEINDEX]+=4;
	size[BRUSHSIZEINDEX] = sizeOfBrush>10?10:size[BRUSHSIZEINDEX];

	if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x - size[BRUSHSIZEINDEX], y - size[BRUSHSIZEINDEX]))
		if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + size[BRUSHSIZEINDEX], y + size[BRUSHSIZEINDEX]))
		{
			glBegin(GL_POLYGON);
			glVertex2f(x + size[BRUSHSIZEINDEX], y + size[BRUSHSIZEINDEX]);
			glVertex2f(x + size[BRUSHSIZEINDEX], y - size[BRUSHSIZEINDEX]);
			glVertex2f(x - size[BRUSHSIZEINDEX], y - size[BRUSHSIZEINDEX]);
			glVertex2f(x - size[BRUSHSIZEINDEX], y + size[BRUSHSIZEINDEX]);
			glEnd();
		}
	size[BRUSHSIZEINDEX] = sizeOfBrush;
}

void sprayStroke(int x, int y)
{
	if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x - size[BRUSHSIZEINDEX], y - size[BRUSHSIZEINDEX]))
		if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + size[BRUSHSIZEINDEX], y + size[BRUSHSIZEINDEX]))
		{
			glPointSize(1);
			glBegin(GL_POINTS);
			int i;
			float minx = x - 3 * size[BRUSHSIZEINDEX];
			float miny = y - 3 * size[BRUSHSIZEINDEX];
			float maxx = x + 3 * size[BRUSHSIZEINDEX];
			float maxy = y + 3 * size[BRUSHSIZEINDEX];

			for(i=0;i < 40 * size[BRUSHSIZEINDEX];i++)
			{
				glVertex2f((float)rand()/RAND_MAX*(maxx- minx) + minx, (float)rand()/RAND_MAX*(maxy - miny) + miny);
			}
			glEnd();
			glPointSize(size[BRUSHSIZEINDEX]);
		}
}

void erazerStroke(int x, int y)
{
	int sizeOfBrush = size[BRUSHSIZEINDEX];
	size[BRUSHSIZEINDEX]+=4;
	size[BRUSHSIZEINDEX] = sizeOfBrush>10?10:size[BRUSHSIZEINDEX];

	if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x - size[BRUSHSIZEINDEX], y - size[BRUSHSIZEINDEX]))
		if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + size[BRUSHSIZEINDEX], y + size[BRUSHSIZEINDEX]))
		{
			glColor3fv(colors[1]);
			glBegin(GL_POLYGON);
			glVertex2f(x + size[BRUSHSIZEINDEX], y + size[BRUSHSIZEINDEX]);
			glVertex2f(x + size[BRUSHSIZEINDEX], y - size[BRUSHSIZEINDEX]);
			glVertex2f(x - size[BRUSHSIZEINDEX], y - size[BRUSHSIZEINDEX]);
			glVertex2f(x - size[BRUSHSIZEINDEX], y + size[BRUSHSIZEINDEX]);
			glEnd();
			glColor3fv(colors[clr]);
		}
	size[BRUSHSIZEINDEX] = sizeOfBrush;
}

/*
	CURVES
	======
	- Spiral
	- Limacon
	- Cardiod
	- Three Leaf
*/

void spiral(int cx, int cy)
{
	float t, x, y;
	if(cx < canvasXMIN || cy < canvasYMIN || cx > canvasXMAX || cy > canvasYMAX)
		return;
	glBegin(GL_LINE_STRIP);
	for(t = 0; t < 3.14 * 10; t += 0.02)
	{
		x = t * cos(t) * 2 + cx;
		y = t * sin(t) * 2 + cy;
		glVertex2f(x, y);
	}
	glEnd();
	glFlush();
}

void limacon(int cx, int cy)
{
	float t, x, y;
	int a = 70, b = 40;
	if(cx < canvasXMIN || cy < canvasYMIN || cx > canvasXMAX || cy > canvasYMAX)
		return;
	glBegin(GL_LINE_LOOP);
	for(t = 0; t < 3.14 * 2; t += 0.02)
	{
		x = (a * cos(t) + b) * cos(t) + cx;
		y = (a * cos(t) + b) * sin(t) + cy;
		glVertex2f(x, y);
	}
	glEnd();
	glFlush();
}

void cardiod(int cx, int cy)
{
	float t, x, y;
	int a = 70;
	if(cx < canvasXMIN || cy < canvasYMIN || cx > canvasXMAX || cy > canvasYMAX)
		return;
	glBegin(GL_LINE_LOOP);
	for(t = 0; t < 3.14 * 2; t += 0.02)
	{
		x = (a * (1 + cos(t))) * cos(t) + cx;
		y = (a * (1 + cos(t))) * sin(t) + cy;
		glVertex2f(x, y);
	}
	glEnd();
	glFlush();
}

void threeLeaf(int cx, int cy)
{
	float t, x, y;
	int a = 70;
	if(cx < canvasXMIN || cy < canvasYMIN || cx > canvasXMAX || cy > canvasYMAX)
		return;
	glBegin(GL_LINE_LOOP);
	for(t = 0; t < 3.14 * 2; t += 0.02)
	{
		x = (a * cos(3 * t)) * cos(t) + cx;
		y = (a * cos(3 * t)) * sin(t) + cy;
		glVertex2f(x, y);
	}
	glEnd();
	glFlush();
}

void drawCube(int x, int y, int size)
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + 0.812 * size, y + size * 0.406);
	glVertex2f(x, y + size * 0.812);
	glVertex2f(x - 0.812 * size, y + size * 0.406);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y + size);
	glVertex2f(x + 0.812 * size, y + size * 1.406);
	glVertex2f(x, y + size * 1.812);
	glVertex2f(x - 0.812 * size, y + size * 1.406);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x, y + size);
	glVertex2f(x + 0.812 * size, y + size * 0.406);
	glVertex2f(x + 0.812 * size, y + size * 1.406);
	glVertex2f(x, y + size * 0.812);
	glVertex2f(x, y + size * 1.812);
	glVertex2f(x - 0.812 * size, y + size * 0.406);
	glVertex2f(x - 0.812 * size, y + size * 1.406);
	glEnd();
	glFlush();
}

void drawFilledCube(int x, int y, int size)
{
	int altcolor = 1;
	if(clr == 1)
		altcolor = 3;
	glBegin(GL_POLYGON);
	glColor3fv(colors[altcolor]);
	glVertex2f(x, y);
	glColor3fv(colors[clr]);
	glVertex2f(x + 0.812 * size, y + size * 0.406);
	glVertex2f(x, y + size * 0.812);
	glVertex2f(x - 0.812 * size, y + size * 0.406);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3fv(colors[altcolor]);
	glVertex2f(x, y + size);
	glColor3fv(colors[clr]);
	glVertex2f(x + 0.812 * size, y + size * 1.406);
	glVertex2f(x, y + size * 1.812);
	glVertex2f(x - 0.812 * size, y + size * 1.406);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3fv(colors[altcolor]);
	glVertex2f(x, y + size);
	glColor3fv(colors[clr]);
	glVertex2f(x, y);
	glVertex2f(x + 0.812 * size, y + size * 0.406);
	glVertex2f(x + 0.812 * size, y + size * 1.406);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3fv(colors[altcolor]);
	glVertex2f(x, y + size);
	glColor3fv(colors[clr]);
	glVertex2f(x, y);
	glVertex2f(x - 0.812 * size, y + size * 0.406);
	glVertex2f(x - 0.812 * size, y + size * 1.406);
	glEnd();

	glColor3fv(colors[altcolor]);
	glBegin(GL_LINES);
	glVertex2f(x, y + size);
	glVertex2f(x + 0.812 * size, y + size * 1.406);
	glVertex2f(x, y + size);
	glVertex2f(x - 0.812 * size, y + size * 1.406);
	glVertex2f(x, y + size);
	glVertex2f(x, y);
	glEnd();
	glColor3fv(colors[clr]);
	glFlush();
}

void drawSphere(int h,int k,int r)
{
	float theta = 0, phi = 0, deltaTheta = PI / 20, deltaPhi = PI / 20;
	float z1, x1, y1, z2, x2, y2, z3, x3, y3, z4, x4, y4;	
	if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, h - r, k))
		if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, h + r, k))
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, h, k + r))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, h, k - r))
				{
					glBegin(GL_LINE_LOOP);

					for(theta = 0; theta <= 2 * PI ; theta += deltaTheta)
					{
						for(phi = -PI; phi <= PI; phi += deltaPhi)
						{
				
							z1 = r * sinf(phi + deltaPhi) * cosf(theta + deltaTheta);
							x1 = r * sinf(phi + deltaPhi) * sinf(theta + deltaTheta);
							y1 = r * cosf(phi + deltaPhi);
				
							z2 = r * sinf(phi) * cosf(theta + deltaTheta);
							x2 = r * sinf(phi) * sinf(theta + deltaTheta);
							y2 = r * cosf(phi);
				
							z3 = r * sinf(phi) * cosf(theta);
							x3 = r * sinf(phi) * sinf(theta);
							y3 = r * cosf(phi);
				
							z4 = r * sinf(phi + deltaPhi) * cosf(theta);
							x4 = r * sinf(phi + deltaPhi) * sinf(theta);
							y4 = r * cosf(phi + deltaPhi);
				
							glColor3fv(colors[clr]);
							glVertex3f(h+x4,k+y4, z4);
							glVertex3f(h+x1,k+y1, z1);
							glVertex3f(h+x2,k+y2, z2);
							glVertex3f(h+x3,k+y3, z3);
						}
					}
					glEnd();
					glFlush();
				}	
}

void drawFilledSphere(int h,int k,int r)
{
	float theta = 0, phi = 0, deltaTheta = PI / 20, deltaPhi = PI / 20;
	float z1, x1, y1, z2, x2, y2, z3, x3, y3, z4, x4, y4;

	if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, h - r, k))
		if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, h + r, k))
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, h, k + r))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, h, k - r))
				{
					glBegin(GL_QUADS);

					for(theta = 0; theta <= 2 * PI ; theta += deltaTheta)
					{
						for(phi = -PI; phi <= PI; phi += deltaPhi)
						{
				
							z1 = r * sinf(phi + deltaPhi) * cosf(theta + deltaTheta);
							x1 = r * sinf(phi + deltaPhi) * sinf(theta + deltaTheta);
							y1 = r * cosf(phi + deltaPhi);
				
							z2 = r * sinf(phi) * cosf(theta + deltaTheta);
							x2 = r * sinf(phi) * sinf(theta + deltaTheta);
							y2 = r * cosf(phi);
				
							z3 = r * sinf(phi) * cosf(theta);
							x3 = r * sinf(phi) * sinf(theta);
							y3 = r * cosf(phi);
				
							z4 = r * sinf(phi + deltaPhi) * cosf(theta);
							x4 = r * sinf(phi + deltaPhi) * sinf(theta);
							y4 = r * cosf(phi + deltaPhi);
				
							int altcolor = 1;
							if(clr == 1)
								altcolor = 0;
				
							glColor3fv(colors[clr]);
							glVertex3f(h+x4,k+y4, z4);
							glColor3fv(colors[clr]);
							glVertex3f(h+x1,k+y1, z1);
							glColor3fv(colors[clr]);
							glVertex3f(h+x2,k+y2, z2);
							glColor3fv(colors[altcolor]);
							glVertex3f(h+x3,k+y3, z3);
						}
					}
					glEnd();
					glFlush();
				}
}

/*
	TEXT
	=====
	- Helvetica 18pt
	- Helvetica 12pt
*/

void writeTextH18(char * text, int x, int y)
{
	glRasterPos2i(x, y);
	while(*text)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*text++);
	}
}

void writeTextH12(char * text, int x, int y)
{
	glRasterPos2i(x, y);
	while(*text)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*text++);
	}
}

/* 
	CALLBACK FUNCTIONS
	==================
		- reshapeCallback()
		- keyboardCallback()
		- displayCallback()
		- mouseCallBack()
		- mouseMotionCallback()
*/

void reshapeCallback(GLint newWidth, GLint newHeight)
{
	// Reshapes the window
	// Input: new width and height of window
	glViewport(0, 0, newWidth, newHeight);
	XMAX = newWidth;
	YMAX = newHeight;
	canvasXMIN = 0.2 * XMAX;
	canvasYMIN = 0.1 * YMAX;
	canvasXMAX = 0.95 * XMAX;
	canvasYMAX = 0.9 * YMAX;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, XMAX, 0, YMAX, -2000, 2000);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
	glFlush();
}

void keyboardCallback(unsigned char key, int x, int y)
{
	// Handles response to keys entered on a keyboard
	// Input: key enterted and x and y coordinates of the mouse

	if(key == 'b' || key =='B')
	{	
		sizeIndex = BRUSHSIZEINDEX;
		printf("Changing brush size\n");
	}
	else if(key == 'l' || key =='L')
	{
		sizeIndex = LINESIZEINDEX;
		printf("Changing line size\n");
	}
	else if(key == '+')
	{
		if(size[sizeIndex] == 10)
		{
			printf("Cannot increase size any further.\n");
			return;
		}
		size[sizeIndex] += 1;
		if(sizeIndex == BRUSHSIZEINDEX)
			glPointSize(size[sizeIndex]);
		else if(sizeIndex == LINESIZEINDEX)
			glLineWidth(size[sizeIndex]);

	}
	else if(key == '-')
	{
		if(size[sizeIndex] == 1)
		{
			printf("Cannot reduce size any further.\n");
			return;
		}
		size[sizeIndex] -= 1;
		if(sizeIndex == BRUSHSIZEINDEX)
			glPointSize(size[sizeIndex]);
		else if(sizeIndex == LINESIZEINDEX)
			glLineWidth(size[sizeIndex]);
	}
}

void handleColorSelection(int x, int y)
{
	float dimension;
	int i;

	if(XMAX < YMAX)
		dimension = XMAX;
	else
		dimension = YMAX;

	for(i = 0; i < numberOfColorColumns; i++)
	{
		if(inside_area(0.1 * dimension + 0.05 * dimension * i, 0.1 * dimension + 0.05 * dimension * (i + 1), (0.1 * dimension) / 2, 0.1 * dimension - padding, x, y))
		{
			clr = 2 * i;
			break;
		}
		else if(inside_area(0.1 * dimension + 0.05 * dimension * i, 0.1 * dimension + 0.05 * dimension * (i + 1), padding, (0.1 * dimension) / 2, x, y))
		{
			clr = 2 * i + 1;
			break;
		}
	}
	drawMajorColorBox(clr);
	glFlush();
}

void handleMenuBar(int x, int y)
{
	int i, option = -1;
	char filename[30];
	FILE * fp;

	for(i = 0; i < numberOfMenuItems; i++)
	{
		if(inside_area(i * 0.1 * XMAX, (i + 1) * 0.1 * XMAX, 0.95 * YMAX, YMAX, x, y))
		{
			option = i;
			drawAllOptionBoxes();
			drawMenuBar();
			highLightMenuOption(i);
			labelMenuBar();
			glFlush();
			break;
		}
	}
	if(option == -1)
		return;
	option += 100;
	
	switch(option)
	{
		case NEW:	operation = -1;
					glutPostRedisplay();
					break;
		case SAVE:	glReadPixels(canvasXMIN , canvasYMIN , canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
					printf("Enter the filename to save : ");
					scanf("%s", filename);
					fp = fopen(filename, "w");			
					fwrite(matrix, matrixSize, sizeof(GLubyte), fp);
					fclose(fp);
					printf("\nContents saved in file %s\n", filename);	
					break;
		case OPEN:	printf("\nEnter filename to open : ");
					scanf("%s", filename);
					fp = fopen(filename, "r");
					if(!fp)
					{
						printf("No such file file found.\n");
						return;
					}	
					fread(matrix, matrixSize, sizeof(GLubyte), fp);
					fclose(fp);
					glRasterPos2i(canvasXMIN, canvasYMIN);
					glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
					printf("\nFile %s has been opened successfully\n", filename);
					break;
		case EXIT:	exit(0);
					break;
	}
}

void handleLeftOptions(int x, int y)
{
	int i, option = -1;
	for(i = 0; i < numberOfLeftOptionItems; i++)
	{
		if(inside_area(padding, 0.1 * XMAX - padding, 0.9 * YMAX - optionHeight * (i + 1) - padding * i, 0.9 * YMAX - (optionHeight + padding) * i, x, y))
		{
			option = i;
			drawAllOptionBoxes();
			drawMenuBar();
			highLightLeftOptionBox(i);
			labelLeftOptionBoxes();
			break;
		}
	}
	if(option == -1)
		return;
	option += 200;
	operation = option;
}

void handleRightOptions(int x, int y)
{
	int i, option = -1;
	for(i = 0; i < numberOfRightOptionItems; i++)
	{
		if(inside_area(0.1 * XMAX, 0.2 * XMAX - padding, 0.9 * YMAX - optionHeight * (i + 1) - padding * i, 0.9 * YMAX - (optionHeight + padding) * i, x, y))
		{
			option = i;
			drawAllOptionBoxes();
			drawMenuBar();
			highLightRightOptionBox(i);
			labelRightOptionBoxes();
			break;
		}
	}
	if(option == -1)
		return;
	option += 300;
	operation = option;
}

void floodfill(float pointx,float pointy,float f[3],float o[3])
{
    float pixels[3];
	if(pointx < canvasXMIN || pointx > canvasXMAX || pointy < canvasYMIN || pointy > canvasYMAX)
		return;
    glReadPixels(pointx,pointy,1.0,1.0,GL_RGB,GL_FLOAT,pixels);
    if(pixels[0]==o[0] && (pixels[1])==o[1] && (pixels[2])==o[2])
    {
        glBegin(GL_POINTS);
        glColor3fv(f);
        glVertex2i(pointx,pointy);
        glEnd();
        glFlush();
        floodfill(pointx+1,pointy,f,o);
        floodfill(pointx,pointy+1,f,o);
        floodfill(pointx-1,pointy,f,o);
        floodfill(pointx,pointy-1,f,o);
    }
}

void mouseCallback(int button, int state, int x, int y)
{
	// Function to handle mouse clicks
	// Input: Mouse button clicked, state of the button and x,y coordinates of the clicke event

	y = YMAX - y;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(translateDone)
		{
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + (oldxMax - oldx), y + (oldyMax - oldy)))
				{
					if(oldxMax < oldx)
					{
						oldxMax = oldxMax + oldx;
						oldx = oldxMax - oldx;
						oldxMax = oldxMax - oldx;
					}
					if(oldyMax < oldy)
					{
						oldyMax = oldyMax + oldy;
						oldy = oldyMax - oldy;
						oldyMax = oldyMax - oldy;
					}
					glReadPixels(oldx, oldy + 1, oldxMax - oldx - 1, oldyMax - oldy - 1, GL_RGB, GL_FLOAT, matrix);
					glRasterPos2i(x, y);
					glColor3fv(colors[1]);
					drawFilledRectangle(oldx - 1, oldy, oldxMax, oldyMax + 1);
					glColor3fv(colors[clr]);
					glDrawPixels(oldxMax - oldx - 1, oldyMax - oldy - 1, GL_RGB, GL_FLOAT, matrix);
					oldx = oldy = oldxMax = oldyMax = translateDone= 0;
					glFlush();
				}
		}
		if(scaleDone)
		{
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + (oldxMax - oldx), y + (oldyMax - oldy)))
				{
					if(oldxMax < oldx)
					{
						oldxMax = oldxMax + oldx;
						oldx = oldxMax - oldx;
						oldxMax = oldxMax - oldx;
					}
					if(oldyMax < oldy)
					{
						oldyMax = oldyMax + oldy;
						oldy = oldyMax - oldy;
						oldyMax = oldyMax - oldy;
					}
					glReadPixels(oldx, oldy + 1, oldxMax - oldx - 1, oldyMax - oldy - 1, GL_RGB, GL_FLOAT, matrix);
					glRasterPos2i(canvasXMIN, canvasYMIN);
					glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
					glRasterPos2i(x, y);
					glPixelZoom(2, 2);
					glDrawPixels(oldxMax - oldx - 1, oldyMax - oldy - 1, GL_RGB, GL_FLOAT, matrix);
					oldx = oldy = oldxMax = oldyMax = scaleDone= 0;
					glPixelZoom(1, 1);
					glFlush();					
				}
		}
		if(shearDone)
		{
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + (oldxMax - oldx), y + (oldyMax - oldy)))
				{
					int i,j;
					if(oldxMax < oldx)
					{
						oldxMax = oldxMax + oldx;
						oldx = oldxMax - oldx;
						oldxMax = oldxMax - oldx;
					}
					if(oldyMax < oldy)
					{
						oldyMax = oldyMax + oldy;
						oldy = oldyMax - oldy;
						oldyMax = oldyMax - oldy;
					}
					glReadPixels(oldx, oldy, 500, 500, GL_RGB, GL_FLOAT, clipmatrix);
					glRasterPos2i(canvasXMIN, canvasYMIN);
					glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
					glColor3fv(colors[clr]);
					for(i = 0;i<oldxMax - oldx - 1; i++)
					{
						for(j = 1;j<oldyMax - oldy; j++)
						{
							glColor3fv(clipmatrix[j][i]);
							drawPoint(x+i+shearX*j, y+j+shearY*i);
						}
					}
					glColor3fv(colors[clr]);
					oldx = oldy = oldxMax = oldyMax = shearDone= 0;
					glFlush();
				}
		}
		if(rotateDone)
		{
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + (oldxMax - oldx), y + (oldyMax - oldy)))
				{
					int i,j;
					if(oldxMax < oldx)
					{
						oldxMax = oldxMax + oldx;
						oldx = oldxMax - oldx;
						oldxMax = oldxMax - oldx;
					}
					if(oldyMax < oldy)
					{
						oldyMax = oldyMax + oldy;
						oldy = oldyMax - oldy;
						oldyMax = oldyMax - oldy;
					}
					glReadPixels(oldx, oldy, 500, 500, GL_RGB, GL_FLOAT, clipmatrix);
					glRasterPos2i(canvasXMIN, canvasYMIN);
					glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
					glColor3fv(colors[clr]);
					glPointSize(2);
					for(i = 0;i<oldxMax - oldx - 1; i++)
					{
						for(j = 1;j<oldyMax - oldy; j++)
						{
							glColor3fv(clipmatrix[j][i]);
							drawPoint((i) * cos(rotationAngle) - (j) * sin(rotationAngle) + x, (i) * sin(rotationAngle) + (j) * cos(rotationAngle) + y);
						}
					}
					glPointSize(1);
					glColor3fv(colors[clr]);
					oldx = oldy = oldxMax = oldyMax = rotateDone= 0;
					glFlush();
				}
		}
		if(reflectDone)
		{
			if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
				if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x + (oldxMax - oldx), y + (oldyMax - oldy)))
				{
					int i,j;
					if(oldxMax < oldx)
					{
						oldxMax = oldxMax + oldx;
						oldx = oldxMax - oldx;
						oldxMax = oldxMax - oldx;
					}
					if(oldyMax < oldy)
					{
						oldyMax = oldyMax + oldy;
						oldy = oldyMax - oldy;
						oldyMax = oldyMax - oldy;
					}
					glReadPixels(oldx, oldy, 500, 500, GL_RGB, GL_FLOAT, clipmatrix);
					glRasterPos2i(canvasXMIN, canvasYMIN);
					glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
					glColor3fv(colors[clr]);
					glPointSize(2);
					float reflectAngle = atan(reflectM) * 180.0 / PI;
					glPushMatrix();
					glTranslatef(0,reflectIntercept,0);
					glRotatef(reflectAngle,0,0,1);
					glScalef(1,-1,1);
					glRotatef(-reflectAngle,0,0,1);
					glTranslatef(0,-reflectIntercept,0);
					for(i = 0;i<oldxMax - oldx - 1; i++)
					{
						for(j = 1;j<oldyMax - oldy; j++)
						{
							glColor3fv(clipmatrix[j][i]);
							drawPoint(oldx+i,oldy+j);							
							// drawPoint((x + i - oldx) * cos(2*rotationAngle) - (y + j - oldy) * sin(2*rotationAngle) + x, (x + i - oldx) * sin(2*rotationAngle) - (x + j -oldy) * cos(2*rotationAngle) + y);
						}
					}
					glPopMatrix();
					glPointSize(1);
					glColor3fv(colors[clr]);
					oldx = oldy = oldxMax = oldyMax = reflectDone= 0;
					glFlush();
				}
		}

		switch(operation)
		{
			case LINE:
			case TRIANGLE:
			case FILLEDTRIANGLE:
			case RECTANGLE:
			case FILLEDRECTANGLE:
			case FILLEDCIRCLE:
			case CIRCLE: 	
			case SPHERE:
			case FILLEDSPHERE:
			case CUBE:
			case FILLEDCUBE:
			case OUTERCLIP:
			case INNERCLIP:
			case SCALE:
			case TRANSLATE:
			case ROTATE:
			case REFLECT:
			case SHEAR:
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								oldx = x;
								oldy = y;
								glReadPixels(canvasXMIN, canvasYMIN, canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
							}
							break;
		}
		handleColorSelection(x, y);
		handleMenuBar(x, y);
		handleLeftOptions(x, y);
		handleRightOptions(x, y);
		switch(operation)
		{
			case PENCIL: 	pencilStroke(x, y);
							break;
			case BRUSH: 	brushStroke(x, y);
							break;
			case SPRAY: 	sprayStroke(x, y);
							break;
			case ERAZER: 	erazerStroke(x, y);
							break;
			case SPIRAL: 	spiral(x, y);
							break;
			case LIMACON: 	limacon(x,y);
							break;
			case CARDIOD: 	cardiod(x, y);
							break;
			case THREELEAF:	threeLeaf(x, y);
							break;
			case FLOODFILL:	float pixels[3];
							glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, pixels);
							glPointSize(1);
							floodfill(x, y, colors[clr], pixels);
							glPointSize(size[BRUSHSIZEINDEX]);
							break;
		}
		glFlush();
	}
	else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		switch(operation)
		{
			case LINE:	if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							drawLine(oldx, oldy, x, y);
							oldx=0;oldy=0;
						}		
						break;
			case TRIANGLE:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								drawTriangle(oldx, oldy, x, y);
								oldx=0;oldy=0;
							}			
							break;
			case FILLEDTRIANGLE:	if(!(oldx|oldy)) 
										break;
									if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
									{
										drawFilledTriangle(oldx, oldy, x, y);
										oldx=0;oldy=0;
									}		
									break;
			case RECTANGLE:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								drawRectangle(oldx, oldy, x, y);
								oldx=0;oldy=0;
							}	
							break;
			case FILLEDRECTANGLE:	if(!(oldx|oldy)) 
										break;
									if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
									{
										drawFilledRectangle(oldx, oldy, x, y);
										oldx=0;oldy=0;
									}
									break;
			case CIRCLE: 	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
								drawCircle(oldx, oldy, r);
								oldx=0;oldy=0;
							}						
							break;
			case FILLEDCIRCLE:	if(!(oldx|oldy)) 
									break;
								if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
								{
									float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
									drawFilledCircle(oldx, oldy, r);
									oldx=0;oldy=0;
								}						
								break;
			case SPHERE: 	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
								drawSphere(oldx, oldy, r);
								oldx=0;oldy=0;
							}						
							break;
			case FILLEDSPHERE:	if(!(oldx|oldy)) 
									break;
								if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
								{
									float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
									drawFilledSphere(oldx, oldy, r);
									oldx=0;oldy=0;
								}
								break;
			case CUBE:	if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
							drawCube(oldx, oldy, r);
							oldx=0;oldy=0;
						}
						break;
			case FILLEDCUBE:	if(!(oldx|oldy)) 
									break;
								if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
								{
									float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
									drawFilledCube(oldx, oldy, r);
									oldx=0;oldy=0;
								}
								break;
			case OUTERCLIP:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								if(oldx > x)
								{
									oldx = oldx + x;
									x = oldx - x;
									oldx = oldx - x;
								}
								if(oldy > y)
								{
									oldy = oldy + y;
									y = oldy - y;
									oldy = oldy - y;
								}
								glReadPixels(oldx, oldy, x - oldx, y - oldy, GL_RGB, GL_FLOAT, matrix);
								glColor3fv(colors[1]);
								drawFilledRectangle(canvasXMIN, canvasYMIN, canvasXMAX, canvasYMAX);
								glRasterPos2i(oldx, oldy);
								glDrawPixels(x - oldx, y - oldy, GL_RGB, GL_FLOAT, matrix);
								drawRectangle(oldx, oldy, x, y);
								glColor3fv(colors[clr]);
								oldx=0;oldy=0;
								glFlush();
							}
							break;
			case INNERCLIP:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								glRasterPos2i(canvasXMIN, canvasYMIN);
								glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
								glColor3fv(colors[1]);
								drawFilledRectangle(oldx, oldy, x, y);
								glColor3fv(colors[clr]);
								oldx=0;oldy=0;
							}		
							break;
			case SCALE:	if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							oldxMax = x;
							oldyMax = y;
							scaleDone = 1;
						}	
						break;
			case TRANSLATE:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								oldxMax = x;
								oldyMax = y;
								translateDone = 1;
							}		
							break;
			case SHEAR:	if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							oldxMax = x;
							oldyMax = y;
							shearDone = 1;
						}	
						break;
			case ROTATE:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								oldxMax = x;
								oldyMax = y;
								rotateDone = 1;
							}	
							break;
			case REFLECT:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								oldxMax = x;
								oldyMax = y;
								reflectDone = 1;
							}	
							break;
		}
	}
}

void mouseMotionCallback(int x, int y)
{
	y = YMAX - y;
	switch(operation)
	{
		case PENCIL:	pencilStroke(x, y);
						break;
		case BRUSH:		brushStroke(x, y);
						break;
		case ERAZER:	erazerStroke(x, y);
						break;
		case SPRAY:		sprayStroke(x, y);
						break;
		case LINE:		if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							glRasterPos2i(canvasXMIN, canvasYMIN);
							glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
							drawLine(oldx, oldy, x, y);
						}							
						break;
		case TRIANGLE:	if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							glRasterPos2i(canvasXMIN, canvasYMIN);
							glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
							drawTriangle(oldx, oldy, x, y);
						}							
						break;
		case FILLEDTRIANGLE:	if(!(oldx|oldy)) 
									break;
								if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
								{
									glRasterPos2i(canvasXMIN, canvasYMIN);
									glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
									drawFilledTriangle(oldx, oldy, x, y);
								}							
								break;
		case FILLEDRECTANGLE:	if(!(oldx|oldy)) 
									break;
								if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
								{
									glRasterPos2i(canvasXMIN, canvasYMIN);
									glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
									drawFilledRectangle(oldx, oldy, x, y);
								}							
								break;
		case RECTANGLE: if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							glRasterPos2i(canvasXMIN, canvasYMIN);
							glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
							drawRectangle(oldx, oldy, x, y);
						}							
						break;
		case CIRCLE:	if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							glRasterPos2i(canvasXMIN, canvasYMIN);
							glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
							float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
							drawCircle(oldx, oldy, r);
						}							
						break;
		case FILLEDCIRCLE: 	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								glRasterPos2i(canvasXMIN, canvasYMIN);
								glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
								float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
								drawFilledCircle(oldx, oldy, r);
							}							
							break;
		case SPHERE:	if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							glRasterPos2i(canvasXMIN, canvasYMIN);
							glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
							float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
							drawSphere(oldx, oldy, r);
						}							
						break;
		case FILLEDSPHERE:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								glRasterPos2i(canvasXMIN, canvasYMIN);
								glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
								float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
								drawFilledSphere(oldx, oldy, r);
							}							
							break;
		case CUBE:	if(!(oldx|oldy)) 
								break;
					if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
					{
						glRasterPos2i(canvasXMIN, canvasYMIN);
						glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
						float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
						drawCube(oldx, oldy, r);
					}							
					break;
		case FILLEDCUBE:	if(!(oldx|oldy)) 
								break;
							if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
							{
								glRasterPos2i(canvasXMIN, canvasYMIN);
								glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
								float r = sqrt((float)((x - oldx) * (x - oldx) + (y - oldy) * (y - oldy)));
								drawFilledCube(oldx, oldy, r);
							}							
							break;
		case OUTERCLIP:
		case INNERCLIP:
		case SCALE:
		case TRANSLATE:
		case SHEAR:
		case ROTATE:
		case REFLECT:
						if(!(oldx|oldy)) 
							break;
						if(inside_area(canvasXMIN, canvasXMAX, canvasYMIN, canvasYMAX, x, y))
						{
							glRasterPos2i(canvasXMIN, canvasYMIN);
							glDrawPixels(canvasXMAX, canvasYMAX, GL_RGB, GL_FLOAT, matrix);
							drawRectangle(oldx, oldy, x, y);
						}
						break;
	}
	glFlush();
}

// Helper functions for display

void drawBorder()
{
	// Draws the border around the canvas

	glClearColor(0.15, 0.4375, 0.68, 1);
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex2f(canvasXMIN, canvasYMIN);
	glVertex2f(canvasXMIN, canvasYMAX);
	glVertex2f(canvasXMAX, canvasYMAX);
	glVertex2f(canvasXMAX, canvasYMIN);
	glEnd();
	glColor3fv(colors[clr]);
}

void displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawBorder();
	glColor3fv(colors[0]);

	drawAllOptionBoxes();
	drawColorPalette();
	drawMenuBar();
	glFlush();
}

void init()
{
	glClearColor(1, 1, 1, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glColor3f(0, 0, 0);
	glOrtho(0, XMAX, 0, YMAX, -2000, 2000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char **argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(XMAX, YMAX);
	glutCreateWindow("Graphics Editor");
	init();
	glutReshapeFunc(reshapeCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotionCallback);
	glutDisplayFunc(displayCallback);
	glutMainLoop();
}