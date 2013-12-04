#include<GL/glut.h>

extern GLint XMAX;
extern GLint YMAX;
extern GLfloat colors[][3];
extern GLint clr;
extern GLint padding;
extern GLint optionHeight;
extern void writeTextH12(char *, int, int);

char leftOptionNames[][20] = {"Pencil", "Brush", "Spray", "Erazer", "Line", "Rectangle", "Filled Rectangle", "Circle", "Triangle", "Filled Triangle"};
int numberOfLeftOptionItems = 10;

char rightOptionNames[][20] = {"Spiral", "Limacon", "Cardiod", "Three Leaf", "Sphere", "Inner Clip", "Outer Clip", "Scale", "Rotate"};
int numberOfRightOptionItems = 9;

void labelLeftOptionBoxes()
{
	int i;
	float offsetForName = 0;
	float yoffset = 2 * padding;

	glColor3fv(colors[1]);
	for(i = 0; i < numberOfLeftOptionItems; i++)
	{
		int length = 0;
		int j = 0;
		for(j = 0; leftOptionNames[i][j] != '\0';j++)
			length++;
		int factor = 8;
		if(length > 10)
			factor = 6;
		offsetForName = (0.1 * XMAX - length * factor)/2;
		writeTextH12(leftOptionNames[i], padding + offsetForName,  0.9 * YMAX - optionHeight * (i + 1) - padding * i + yoffset);
	}
}

void labelRightOptionBoxes()
{
	int i;
	float offsetForName = 0;
	float yoffset = 2 * padding;

	glColor3fv(colors[1]);
	for(i = 0; i < numberOfRightOptionItems; i++)
	{
		int length = 0;
		int j = 0;
		for(j = 0; rightOptionNames[i][j] != '\0';j++)
			length++;
		offsetForName = (0.1 * XMAX - length * 7)/2;
		writeTextH12(rightOptionNames[i], padding + 0.1 * XMAX + offsetForName,  0.9 * YMAX - optionHeight * (i + 1) - padding * i + yoffset);
	}
}

void drawLeftOptionBox(int boxNumber)
{
	glColor3f(0.75, 0.75, 0.75);
	glBegin(GL_POLYGON);
	glVertex2f(padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.1 * XMAX - padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.1 * XMAX - padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glVertex2f(padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glEnd();

	glColor3fv(colors[0]);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.1 * XMAX - padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.1 * XMAX - padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glVertex2f(padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glEnd();
	glLineWidth(1);

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

	glColor3fv(colors[0]);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.1 * XMAX, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.2 * XMAX - padding, 0.9 * YMAX - (optionHeight + padding) * boxNumber);
	glVertex2f(0.2 * XMAX - padding, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glVertex2f(0.1 * XMAX, 0.9 * YMAX - optionHeight * (boxNumber + 1) - padding * boxNumber);
	glEnd();
	glLineWidth(1);

	glColor3fv(colors[clr]);
}

void drawAllOptionBoxes()
{
	int i;

	for(i = 0; i < numberOfLeftOptionItems; i++)
		drawLeftOptionBox(i);

	labelLeftOptionBoxes();

	for(i = 0; i < numberOfRightOptionItems; i++)
		drawRightOptionBox(i);

	labelRightOptionBoxes();

	glLineWidth(1);
	glFlush();
}