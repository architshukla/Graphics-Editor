#include<stdio.h>
#include<math.h>
#include<GL/glut.h>

#include "Boxes.h"
#include "colorPalette.h"
#include "menuBar.h"

#define BRUSHSIZEINDEX 0
#define LINESIZEINDEX 1
#define PI 3.1415

#define NEW 100
#define SAVE 101
#define OPEN 102
#define EXIT 103

#define PENCIL 200
#define BRUSH 201
#define SPRAY 202
#define ERAZER 203
#define LINE 204
#define RECTANGLE 205
#define FILLEDRECTANGLE 206
#define CIRCLE 207
#define TRIANGLE 208
#define FILLEDTRIANGLE 209

#define SPIRAL 300
#define LIMACON 301
#define CARDIOD 302
#define THREELEAF 303
#define SPHERE 304
#define INNERCLIP 305
#define OUTERCLIP 306
#define SCALE 307
#define ROTATE 308

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

extern int numberOfColorColumns, numberOfMenuItems, numberOfLeftOptionItems, numberOfRightOptionItems;

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
	int a = 70, b = 40;
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
	int a = 70, b = 40;
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

void manualDrawSphere(int h,int k,int r)
{
	float theta = 0, phi = 0, deltaTheta = PI / 20, deltaPhi = PI / 20;
	float z1, x1, y1, z2, x2, y2, z3, x3, y3, z4, x4, y4;	
	
	glBegin(GL_QUAD_STRIP);

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

			int altcolor = 0;
			if(clr == 0)
				altcolor = 1;

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
	glOrtho(0, XMAX, 0, YMAX, -200, 200);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
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

int inside_area(float left, float right, float bottom, float top, float x, float y)
{
	if(x>left && x<right)
		if(y<top && y>bottom)
			return 1;
	return 0;
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
	for(i = 0; i < numberOfMenuItems; i++)
	{
		if(inside_area(i * 0.1 * XMAX, (i + 1) * 0.1 * XMAX, 0.95 * YMAX, YMAX, x, y))
		{
			option = i;
			break;
		}
	}
	if(option == -1)
		return;
	option += 100;
	switch(option)
	{
		case NEW: glutPostRedisplay();
			break;
		case SAVE: 
			break;
		case OPEN: 
			break;
		case EXIT: exit(0);
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
			break;
		}
	}
	if(option == -1)
		return;
	option += 200;
	switch(option)
	{
		case PENCIL: printf("At PENCIL\n");
			break;
		case BRUSH: printf("At BRUSH\n");
			break;
		case SPRAY: printf("At SPRAY\n");
			break;
		case ERAZER: printf("At ERAZER\n");
			break;
		case LINE: printf("At LINE\n");
			break;
		case RECTANGLE: printf("At RECTANGLE\n");
			break;
		case FILLEDRECTANGLE: printf("At FILLED RECTANGLE\n");
			break;
		case CIRCLE: printf("At CIRCLE\n");
			break;
		case TRIANGLE: printf("At TRIANGLE\n");
			break;
		case FILLEDTRIANGLE: printf("At FILLED TRIANGLE\n");
			break;
	}
}

void handleRightOptions(int x, int y)
{
	int i, option = -1;
	for(i = 0; i < numberOfRightOptionItems; i++)
	{
		if(inside_area(0.1 * XMAX, 0.2 * XMAX - padding, 0.9 * YMAX - optionHeight * (i + 1) - padding * i, 0.9 * YMAX - (optionHeight + padding) * i, x, y))
		{
			option = i;
			break;
		}
	}
	if(option == -1)
		return;
	option += 300;
	switch(option)
	{
		case SPIRAL: printf("At SPIRAL\n");
			operation = option;		
			break;
		case LIMACON: printf("At LIMACON\n");
			operation = option;
			break;
		case CARDIOD: printf("At CARDIOD\n");
			operation = option;
			break;
		case THREELEAF: printf("At THREELEAF\n");
			operation = option;
			break;
		case SPHERE: printf("At SPHERE\n");
			break;
		case INNERCLIP: printf("At INNERCLIP\n");
			break;
		case OUTERCLIP: printf("At OUTERCLIP\n");
			break;
		case SCALE: printf("At SCALE:\n");
			break;
		case ROTATE: printf("At ROTATE\n");
			break;
	}
}

void mouseCallback(int button, int state, int x, int y)
{
	// Function to handle mouse clicks
	// Input: Mouse button clicked, state of the button and x,y coordinates of the clicke event

	y = YMAX - y;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		handleColorSelection(x, y);
		handleMenuBar(x, y);
		handleLeftOptions(x, y);
		handleRightOptions(x, y);
		switch(operation)
		{
			case SPIRAL: spiral(x, y);
				break;
			case LIMACON: limacon(x,y);
				break;
			case CARDIOD: cardiod(x, y);
				break;
			case THREELEAF: threeLeaf(x, y);
				break;
		}
	}
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
	glOrtho(0, XMAX, 0, YMAX, 200, -200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(XMAX, YMAX);
	glutCreateWindow("Graphics Editor");
	init();
	glutReshapeFunc(reshapeCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutDisplayFunc(displayCallback);
	glutMainLoop();
}