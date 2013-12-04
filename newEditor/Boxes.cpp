#include<GL/glut.h>

extern GLint XMAX;
extern GLint YMAX;
extern GLfloat colors[][3];
extern GLint clr;
extern GLint padding;
extern GLint optionHeight;

void drawLeftOptionBox(int boxNumber)
{
	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_POLYGON);
	glVertex2f(padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.1 * XMAX - padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.1 * XMAX - padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glVertex2f(padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glEnd();
	glColor3fv(colors[clr]);
}

void drawRightOptionBox(int boxNumber)
{
	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_POLYGON);
	glVertex2f(0.1 * XMAX, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.2 * XMAX - padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.2 * XMAX - padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glVertex2f(0.1 * XMAX, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glEnd();
	glColor3fv(colors[clr]);
}

void drawAllOptionBoxes()
{
	drawLeftOptionBox(0);
	drawLeftOptionBox(1);
	drawLeftOptionBox(2);
	drawLeftOptionBox(3);
	drawLeftOptionBox(4);
	drawLeftOptionBox(5);

	drawRightOptionBox(0);
	drawRightOptionBox(1);
	drawRightOptionBox(2);
	drawRightOptionBox(3);
	drawRightOptionBox(4);
	drawRightOptionBox(5);
}