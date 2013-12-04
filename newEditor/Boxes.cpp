#include<GL/glut.h>

extern GLint XMAX;
extern GLint YMAX;
extern GLfloat colors[18][3];
extern GLint clr;
extern GLint padding;
extern GLint optionHeight;

void drawLeftOptionBox(int boxNumber)
{
	glColor3f(0.3, 0.3, 0.3);
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
	glColor3f(0.3, 0.3, 0.3);
	glBegin(GL_POLYGON);
	glVertex2f(0.9 * XMAX + padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(XMAX - padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(XMAX - padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glVertex2f(0.9 * XMAX + padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
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
}