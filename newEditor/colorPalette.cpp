#include<GL/glut.h>

#define TOP 1
#define BOTTOM 0

extern GLint XMAX;
extern GLint YMAX;
extern GLfloat colors[][3];
extern GLint clr;
extern GLint padding;

int numberOfColorColumns = 10;

void drawMajorColorBox(int colorIndex)
{
	int dimension;
	if(XMAX < YMAX)
		dimension = XMAX;
	else
		dimension = YMAX;

	glColor3fv(colors[colorIndex]);
	glBegin(GL_POLYGON);
	glVertex2f(padding, padding);
	glVertex2f(padding, 0.1 * dimension - padding);
	glVertex2f(0.1 * dimension - padding, 0.1 * dimension - padding);
	glVertex2f(0.1 * dimension - padding, padding);
	glEnd();

	glLineWidth(3);
	glColor3fv(colors[0]);
	glBegin(GL_LINE_LOOP);
	glVertex2f(padding, padding);
	glVertex2f(padding, 0.1 * dimension - padding);
	glVertex2f(0.1 * dimension - padding, 0.1 * dimension - padding);
	glVertex2f(0.1 * dimension - padding, padding);
	glEnd();
	glLineWidth(1);

	glColor3fv(colors[clr]);
}

void drawMinorColorBox(int row, int offset, int colorIndex)
{
	int dimension;
	if(XMAX < YMAX)
		dimension = XMAX;
	else
		dimension = YMAX;

	glColor3fv(colors[colorIndex]);
	if(row == TOP)
	{
		glBegin(GL_POLYGON);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset), (0.1 * dimension) / 2);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset), 0.1 * dimension - padding);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset + 1), 0.1 * dimension - padding);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset + 1), (0.1 * dimension) / 2);
		glEnd();

		glColor3fv(colors[0]);
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset), (0.1 * dimension) / 2);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset), 0.1 * dimension - padding);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset + 1), 0.1 * dimension - padding);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset + 1), (0.1 * dimension) / 2);
		glLineWidth(1);
		glEnd();
	}
	else
	{
		glBegin(GL_POLYGON);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset), padding);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset), (0.1 * dimension - padding) / 2);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset + 1), (0.1 * dimension - padding) / 2);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset + 1), padding);
		glEnd();

		glColor3fv(colors[0]);
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset), padding);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset), (0.1 * dimension - padding) / 2);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset + 1), (0.1 * dimension - padding) / 2);
		glVertex2f(0.1 * dimension + 0.05 * dimension * (offset + 1), padding);
		glLineWidth(1);
		glEnd();
	}
	glColor3fv(colors[clr]);
}

void drawOutlineOfColorBoxes()
{
	int dimension;
	if(XMAX < YMAX)
		dimension = XMAX;
	else
		dimension = YMAX;

	glColor3fv(colors[0]);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.1 * dimension, padding);
	glVertex2f(0.1 * dimension, 0.1 * dimension - padding);
	glVertex2f(0.1 * dimension + 0.05 * dimension * numberOfColorColumns, 0.1 * dimension - padding);
	glVertex2f(0.1 * dimension + 0.05 * dimension * numberOfColorColumns, padding);
	glEnd();
	glColor3fv(colors[clr]);
}

void drawColorPalette()
{
	int i;
	drawMajorColorBox(clr);
	for(i = 0; i < numberOfColorColumns; i++)
	{
		drawMinorColorBox(TOP, i, 2 * i);
		drawMinorColorBox(BOTTOM, i , 2 * i + 1);
	}
	drawOutlineOfColorBoxes();
	glLineWidth(1);
	glFlush();
}