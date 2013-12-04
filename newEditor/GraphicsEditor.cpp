#include<stdio.h>
#include<GL/glut.h>

#include "Boxes.h"
#include "colorPalette.h"
#include "menuBar.h"

#define BRUSHSIZEINDEX 0
#define LINESIZEINDEX 1

GLint XMAX = 1018;
GLint YMAX = 700;
int size[2] = {1, 1};
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

void writeText(char * text, int x, int y)
{
	glRasterPos2i(x, y);
	while(*text)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*text++);
	}
}

/* 
	Callback Functions:
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

void mouseCallback(int button, int state, int x, int y)
{
	// Function to handle mouse clicks
	// Input: Mouse button clicked, state of the button and x,y coordinates of the clicke event

	y = YMAX - y;
	drawPoint(x, y);
}

// Helper functions for display

void drawBorder()
{
	// Draws the border around the canvas

	glClearColor(0.15, 0.4375, 0.68, 1);
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex2f(0.2 * XMAX, 0 + YMAX/10.0);
	glVertex2f(0.95 * XMAX, 0 + YMAX/10.0);
	glVertex2f(0.95 * XMAX, YMAX - YMAX/10.0);
	glVertex2f(0.2 * XMAX, YMAX - YMAX/10.0);
	glEnd();
	glColor3fv(colors[clr]);
}

void displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	clr = 0;
	drawBorder();
	// writeText("Pencil", padding*3, 0.9*YMAX-20);
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
	glEnable(GL_DEPTH_TEST);
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(XMAX, YMAX);
	glutCreateWindow("Graphics Editor");
	init();
	glutReshapeFunc(reshapeCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutDisplayFunc(displayCallback);
	glutMainLoop();
}