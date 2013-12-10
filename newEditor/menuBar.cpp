#include<GL/glut.h>

extern GLint XMAX;
extern GLint YMAX;
extern GLfloat colors[][3];
extern GLint clr;
extern GLint padding;

int numberOfMenuItems = 4;
char menuItemNames[][20] = {"New", "Open", "Save", "Exit"};

extern void writeTextH18(char *, int, int);

void labelMenuBar()
{
	int i;
	float offsetForName = 0;
	
	glColor3fv(colors[1]);
	for(i = 0; i < numberOfMenuItems; i++)
	{
		int length = 0;
		int j = 0;
		for(j = 0; menuItemNames[i][j] != '\0';j++)
			length++;
		offsetForName = (0.1 * XMAX - length * 10)/2;
		writeTextH18(menuItemNames[i], i * 0.1 * XMAX + offsetForName, 0.965 * YMAX);
	}
}

void outlineMenuItem(int i)
{
	glLineWidth(3);
	glBegin(GL_LINE_LOOP);
	glVertex2f(i * 0.1 * XMAX, 0.95 * YMAX);
	glVertex2f((i + 1) * 0.1 * XMAX, 0.95 * YMAX);
	glVertex2f((i + 1) * 0.1 * XMAX, YMAX);
	glVertex2f(i * 0.1 * XMAX, YMAX);
	glEnd();
	glLineWidth(1);
}

void highLightMenuOption(int i)
{
	glColor3fv(colors[6]);
	glBegin(GL_POLYGON);
	glVertex2f(i * 0.1 * XMAX, 0.95 * YMAX);
	glVertex2f((i + 1) * 0.1 * XMAX, 0.95 * YMAX);
	glVertex2f((i + 1) * 0.1 * XMAX, YMAX);
	glVertex2f(i * 0.1 * XMAX, YMAX);
	glEnd();
	glColor3fv(colors[clr]);
}

void drawMenuBar()
{
	int i;

	glColor3f(0.65,0.65,0.65);
	glBegin(GL_POLYGON);
	glVertex2f(0, YMAX);
	glVertex2f(0, 0.95 * YMAX);
	glVertex2f(XMAX, 0.95 * YMAX);
	glVertex2f(XMAX, YMAX);
	glEnd();

	glColor3fv(colors[0]);

	
	for(i = 0; i < numberOfMenuItems; i++)
	{
		outlineMenuItem(i);		
	}
	
	labelMenuBar();

	glColor3fv(colors[clr]);
	glFlush();
}