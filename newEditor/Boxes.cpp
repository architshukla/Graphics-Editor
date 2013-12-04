#include<GL/glut.h>

extern GLint XMAX;
extern GLint YMAX;
extern GLfloat colors[][3];
extern GLint clr;
extern GLint padding;
extern GLint optionHeight;

void drawLeftOptionBox(int boxNumber)
{
	glColor3fv(colors[0]);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.1 * XMAX - padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.1 * XMAX - padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glVertex2f(padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glEnd();
	glLineWidth(1);

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
	glColor3fv(colors[0]);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.1 * XMAX, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.2 * XMAX - padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.2 * XMAX - padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glVertex2f(0.1 * XMAX, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glEnd();
	glLineWidth(1);

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
	int i;
	for(i = 0; i < 8; i++)
		drawLeftOptionBox(i);

	for(i = 0; i < 8; i++)
		drawRightOptionBox(i);
	glLineWidth(1);
	glFlush();
}