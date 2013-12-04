#include<GL/glut.h>

extern GLint XMAX;
extern GLint YMAX;
extern GLfloat colors[][3];
extern GLint clr;
extern GLint padding;

int numberOfMenuItems = 3;
char menuItemNames[][10] = {"New", "Open", "Save"};

extern void writeText(char *, int, int);

void drawMenuBar()
{
	int i;
	float offsetForNames = (0.1 * XMAX) / 3.5;
	
	glColor3fv(colors[1]);
	for(i = 0; i < numberOfMenuItems; i++)
	{
		writeText(menuItemNames[i], i * 0.1 * XMAX + offsetForNames, 0.965 * YMAX);
	}

	glLineWidth(3);
	glColor3fv(colors[0]);
	for(i = 0; i < numberOfMenuItems; i++)
	{

		glBegin(GL_LINE_LOOP);
		glVertex2f(i * 0.1 * XMAX, 0.95 * YMAX);
		glVertex2f((i + 1) * 0.1 * XMAX, 0.95 * YMAX);
		glVertex2f((i + 1) * 0.1 * XMAX, YMAX);
		glVertex2f(i * 0.1 * XMAX, YMAX);
		glEnd();
	}
	glLineWidth(1);

	glColor3f(0.65,0.65,0.65);
	glBegin(GL_POLYGON);
	glVertex2f(0, YMAX);
	glVertex2f(0, 0.95 * YMAX);
	glVertex2f(XMAX, 0.95 * YMAX);
	glVertex2f(XMAX, YMAX);
	glEnd();

	glColor3fv(colors[clr]);
	glFlush();
}