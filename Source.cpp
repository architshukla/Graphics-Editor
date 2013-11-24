#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<GL/glut.h>

#define PENCIL 1
#define BRUSH 2
#define LINE 3
#define RECT 4
#define FILLRECT 5
#define SPRAY 6
#define CIRCLE 7
#define ERAZE 8
#define QUAD 9 
#define NEW 10
#define OPEN 11
#define SAVE 12
#define EXIT 13
#define INSIDECLIP 14
#define OUTSIDECLIP 15
#define TRANSLATE 16
#define SCALING 17
#define SPIRAL 18
#define ERAZE1 19
#define ERAZE2 20
#define ERAZE3 21
#define TRANSLATE1 22
#define ROTATION 23
#define MAXX 1018
#define MAXY 700

int linex=0,liney=0,elinex=0,eliney=0,clr=0,count=0;
int drawline,drawrect,drawcircle,insideclip,outsideclip,translate,scaling,trans_paste,scale_paste,translate1,trans_paste1,rotate,rotate_paste;
int ax,ay,bx,by,px,py;
int state1,size=20,ret,savec,openc;
float arr[MAXX-50][MAXY-50][3],mat[MAXX-50][MAXY-40][3],clip[MAXX-50][MAXY-40][3],mat1[MAXX-50][MAXY-40][3], arr1[MAXX-50][MAXY-50][3];
GLfloat colors[18][3]={{0.0,0.0,0.0},{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,1.0,0.0},{1.0,0.0,1.0},{0.0,1.0,1.0},{1.0,1.0,1.0},{0.75,0.8,0.9},{0.6,0.2,0.7},{0.3,0.3,0.3},{0.1,0.55,0.1},{1.0,0.3,0.0},{0.5,0.4,1},{0.6,0.0,0.1},{0.5,0.2,0.1},{0.7,0.7,0.7},{0.85,0.89,1.0}};
long matsize=(MAXX-50)*(MAXY-40)*3;
FILE *fp;
int screenwidth=MAXX,screenheight=MAXY;


void lineloop(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(x1,y1);
    glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glVertex2i(x4,y4);
	glEnd();
	glFlush();
}

void polygon(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	glBegin(GL_POLYGON);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glVertex2i(x4,y4);
	glEnd();
	glFlush();
}

void line(int x1,int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
	glFlush();
}

void draw_text(char *info,int i,int j)
{
	glRasterPos2i(i,j);
	while(*info)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*info++);
	}
}

void boxes(int x,int y,GLfloat color1[])
{
	glColor3fv(color1);
	polygon(970+5,x,970+5,y,970+45,y,970+45,x);
}
void boxes1(int x,int y,GLfloat color1[])
{
	glColor3fv(color1);
	polygon(5,x,5,y,45,y,45,x);
}
void triangle(int x1,int y1,int x2,int y2,int x3,int y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glEnd();
}
void boxes2(int x,int y,GLfloat color1[])
{
	glColor3fv(color1);
	triangle(970+5,x,970+45,y,970+45,x);
}
void boxes3(int x,int y,GLfloat color1[])
{
	glColor3fv(color1);
	triangle(5,x,45,y,45,x);
}
void boxes4(int x,int y,GLfloat color1[])
{
	glColor3fv(color1);
	triangle(970+5,x,970+45,y,970+45,x);
	glColor3f(1,0.7,0.8);
			polygon(970+20,365,970+20,370,970+30,370,970+30,365);
			polygon(970+15,360-35,970+15,375-35,970+35,375-35,970+35,360-35);
			polygon(970+10,355-70,970+10,380-70,970+40,380-70,970+40,355-70);
}

void menu(int x1, int x2, int y1, int y2, GLfloat color1[])
{
	glColor3fv(color1);
	polygon(x1,y1,x1,y2,x2,y2,x2,y1);	
}

int inside_area(int left,int right,int bottom,int top,int x,int y)
{
	if(x>left && x<right)
		if(y<top && y>bottom)
			return true;
	return false;
}

void draw_pixel(GLint cx,GLint cy)
{
	glBegin(GL_POINTS);
	glVertex2i(cx,cy);
	glEnd();

}

void plot_pixels(GLint h,GLint k,GLint x,GLint y)
{
	draw_pixel(x+h,y+k);
	draw_pixel(-x+h,y+k);
	draw_pixel(x+h,-y+k);
	draw_pixel(-x+h,-y+k);
	draw_pixel(y+h,x+k);
	draw_pixel(-y+h,x+k);
	draw_pixel(y+h,-x+k);
	draw_pixel(-y+h,-x+k);
}

void circle(GLint h,GLint k,GLint r)
{
	GLint d=1-r,x=0,y=r;
	while(y>x)
	{
		plot_pixels(h,k,x,y);
		if(d<0)	d+=2*x+3;
		else
		{
			d+=2*(x-y)+5;
			--y;
		}
		++x;
	}
	plot_pixels(h,k,x,y);
	
}

void rect_draw()
{
	glColor3fv(colors[3]);
	lineloop(970+12,535,970+38,535,970+38,550,970+13,550);
}

void fill_draw()
{
	glColor3fv(colors[0]);
	polygon(970+12,500,970+38,500,970+38,515,970+12,515);	
}


void circle_draw()
{
	glColor3fv(colors[0]);
	circle(970+25,437.5,9);
}

void pencil()
{
	glColor3fv(colors[6]);
	polygon(970+20,650,970+40,650,970+40,643,970+20,643);
	glColor3fv(colors[0]);
	glBegin(GL_TRIANGLES);
	glVertex2f(970+7,647);
	glVertex2f(970+20,652);
	glVertex2f(970+20,643);
	glEnd();
	glColor3fv(colors[0]);
	glPointSize(3.0);
	lineloop(970+20,650,970+40,650,970+40,643,970+20,643);
	/*glBegin(GL_LINE_LOOP);
	glVertex2f(970+25,640);
	glVertex2f(970+35,650);
	glVertex2f(970+30,655);
	glVertex2f(970+20,645);*/
	glEnd();	
}

void putpixel(int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
	glFlush();
}

void spiral(int cx, int cy)
{
	GLfloat t,x3,y3;
	for(t=0;t<6.28*5;t+=0.02)
	{
		x3=t*cos(t)*2+cx;
		y3=t*sin(t)*2+cy;
		putpixel(x3,y3);
		glFlush();
	}
}

void brush()
{
	glColor3f(0.4,0.4,1.0);
	polygon(970+25,608,970+35,618,970+30,623,970+20,613);
	glColor3f(1.0,1.0,0.7);
	polygon(970+25,608,970+20,613,970+12,609,970+21,601);
	glColor3fv(colors[0]);
	glBegin(GL_LINE_LOOP);
	glVertex2f(970+25,608);
	glVertex2f(970+20,613);
	glVertex2f(970+12,609);
	glVertex2f(970+21,601);
	glEnd();	
}

void line()
{
	glColor3fv(colors[3]);
	glBegin(GL_LINE_LOOP);
	glVertex2f(970+17,570);
	glVertex2f(970+35,585);
	glEnd();
}

void eraser()
{
	glColor3f(1,0.7,0.8);
	polygon(970+18,395,970+32,395,970+32,409,970+18,409);
}

void spray_draw()
{
	glColor3fv(colors[3]);
	polygon(970+22,474,970+27,479,970+37,469,970+32,464);
	glBegin(GL_LINE_LOOP);
	glVertex2f(970+22,474);
	glVertex2f(970+27,478);
	glVertex2f(970+19,480);
	glEnd();
	glFlush();
	glColor3fv(colors[0]);
	glPointSize(1.0);
	glBegin(GL_POINTS);
	glVertex2i(970+16,478);
	glVertex2i(970+16,480);
	glVertex2i(970+14,476);
	glVertex2i(970+14,478);
	glVertex2i(970+14,480);
	glVertex2i(970+12,476);
	glVertex2i(970+12,480);
	glVertex2i(970+12,478);
	glVertex2i(970+10,476);
	glVertex2i(970+10,478);
	glVertex2i(970+10,480);
	glVertex2i(970+17,480);
	glEnd();
	glFlush();
}

void disp()
{
	pencil();
	brush();
	line();
	eraser();
	spray_draw();
	circle_draw();
	rect_draw();
	fill_draw();
	glColor3fv(colors[3]);
	draw_text("Inside",12,368);
	draw_text("Clip",15,358);
	draw_text("Outside",7,333);
	draw_text("Clip",15,323);
	draw_text("Translate",5,293);
	draw_text("Scaling",8,258);
	draw_text("Rotation",5,223);
	draw_text("Reflection",5,188);
	glColor3fv(colors[clr]);
	polygon(10,10,10,40,40,40,40,10);	
}

void menu_disp()
{
	glColor3f(0.0,0.0,0.0);
	draw_text("New",13,MAXY-20);
    draw_text("Open",60,MAXY-20);
    draw_text("Save",110,MAXY-20);
    draw_text("Exit",160,MAXY-20);
    draw_text("Cut",210,MAXY-15);
	draw_text("Paste",210,MAXY-25);
	draw_text("Copy",260,MAXY-15);
	draw_text("Paste",260,MAXY-25);
	
	/*draw_text("Inside",210,MAXY-15);
	draw_text("Clip",213,MAXY-25);
	draw_text("Outside",260,MAXY-15);
	draw_text("Clip",265,MAXY-25);
	draw_text("Translate",315,MAXY-20);
	draw_text("Scaling",370,MAXY-20);*/
	glFlush();
}

void spray(int x,int y)
{
	int randx,randy;
	int x1,y1;
	x1=x+10;
	y1=y+15;
	glBegin(GL_POINTS);
	for(int i=0;i<50;i++)
	{
		randx=x1-size+rand()%(size/2);
		randy=y1-size+rand()%(size/2);
		glVertex2i(randx,randy);
	}
	glFlush();
	glEnd();
}

void mouse(int button,int state,int x,int y)
{		
	if(state==GLUT_DOWN && button==GLUT_LEFT_BUTTON)
	{
		y=MAXY-y;
		if(inside_area(50,75,5,25,x,y))
		{
			clr=7;
			glColor3fv(colors[7]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200+50,200+75,5,25,x,y))
		{
			clr=0;
			glColor3fv(colors[0]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}

        if(inside_area(75,100,5,25,x,y))
		{
			clr=16;
			glColor3fv(colors[16]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200+75,200+100,5,25,x,y))
		{
			clr=10;
			glColor3fv(colors[10]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(100,125,5,25,x,y))
		{
			clr=6;
			glColor3fv(colors[6]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200+100,200+125,5,25,x,y))
		{
			clr=3;
			glColor3fv(colors[3]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(125,150,5,25,x,y))
		{
			clr=2;
			glColor3fv(colors[2]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200+125,200+150,5,25,x,y))
		{
			clr=11;
			glColor3fv(colors[11]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}		
		if(inside_area(150,175,5,25,x,y))
		{
			clr=13;
			glColor3fv(colors[13]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200+150,200+175,5,25,x,y))
		{
			clr=9;
			glColor3fv(colors[9]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(175,200,5,25,x,y))
		{
			clr=15;
			glColor3fv(colors[15]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200+175,200+200,5,25,x,y))
		{
			clr=14;
			glColor3fv(colors[14]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}		
		if(inside_area(200,225,5,25,x,y))
		{
			clr=5;
			glColor3fv(colors[5]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200+200,200+225,5,25,x,y))
		{
			clr=1;
			glColor3fv(colors[1]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(225,250,5,25,x,y))
		{
			clr=4;
			glColor3fv(colors[4]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}
		if(inside_area(200+225,200+250,5,25,x,y))
		{
			clr=12;
			glColor3fv(colors[12]);
			polygon(10,10,10,40,40,40,40,10);
			return;
		}		
		int i=65,j=40;
		if(inside_area(970+5,970+45,635,660,x,y))
		{
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes2(635,660,colors[8]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("PENCIL - used for free hand drawing",625,30);
			disp();
			glFlush();
			state1=PENCIL;return;
		}
		i+=35;j+=35;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes2(600,625,colors[8]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("BRUSH - used for free hand drawing with size larger than pencil",625,30);
			disp();
			glFlush();
			state1=BRUSH;return;
		}
		i+=35;j+=35;
        if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes2(565,590,colors[8]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("LINE - used to draw a line of any length between two desired points",625,30);
			disp();
			glFlush();
			state1=LINE;return;
		}		
		i+=35;j+=35;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes2(530,555,colors[8]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("RECTANGLE - used to draw empty rectangle of desired size",625,30);
			disp();
			glFlush();
			state1=RECT;return;
		}
		i+=35;j+=35;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes2(495,520,colors[8]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("FILLED RECTANGLE - used to draw rectangle filled with selected color ",625,30);
			disp();
			glFlush();
			state1=FILLRECT;return;
		}
		i+=35;j+=35;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes2(460,485,colors[8]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("SPRAY - press + to increase size , press - to decrease size",625,30);
			disp();
			glFlush();
			state1=SPRAY;return;
		}		
		i+=35;j+=35;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
            glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes2(425,450,colors[8]);
			boxes(390,415,colors[7]);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("CIRCLE - used to draw circle of desired radius",625,30);
			disp();
			glFlush();
			state1=CIRCLE;
			return;
		}			
		i+=35;j+=35;
		int ii=i,jj=j;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes2(390,415,colors[8]);
			boxes(355,380,colors[7]);
			boxes(320,345,colors[7]);
			boxes(285,310,colors[7]);
			
			glColor3f(1,0.7,0.8);
			polygon(970+20,365,970+20,370,970+30,370,970+30,365);
			polygon(970+15,360-35,970+15,375-35,970+35,375-35,970+35,360-35);
			polygon(970+10,355-70,970+10,380-70,970+40,380-70,970+40,355-70);

			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("ERAZER - used to eraze parts of drawing",625,30);
			disp();
			glFlush();
			state1=ERAZE;
			return;
		}
		i+=35;j+=35;
        if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes2(390,415,colors[8]);
			boxes4(355,380,colors[8]);
			boxes(320,345,colors[7]);
			boxes(285,310,colors[7]);
			glColor3f(1,0.7,0.8);
			//polygon(970+20,365,970+20,370,970+30,370,970+30,365);
			polygon(970+15,360-35,970+15,375-35,970+35,375-35,970+35,360-35);
			polygon(970+10,355-70,970+10,380-70,970+40,380-70,970+40,355-70);

			//boxes(355,380,colors[7]);
			//boxes(320,345,colors[7]);
			//boxes(285,310,colors[7]);
			//boxes(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("SMALL ERAZER",625,30);
			disp();
			glFlush();
			state1=ERAZE1;
			return;
		}
        i+=35;j+=35;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes2(390,415,colors[8]);
			boxes(355,380,colors[7]);
			boxes4(320,345,colors[8]);
			boxes(285,310,colors[7]);
			glColor3f(1,0.7,0.8);
			polygon(970+20,365,970+20,370,970+30,370,970+30,365);
			//polygon(970+15,360-35,970+15,375-35,970+35,375-35,970+35,360-35);
			polygon(970+10,355-70,970+10,380-70,970+40,380-70,970+40,355-70);

			//boxes(355,380,colors[7]);
			//boxes(320,345,colors[7]);
			//boxes(285,310,colors[7]);
			//boxes(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("MEDIUM ERAZER",625,30);
			disp();
			glFlush();
			state1=ERAZE2;
			return;
		}
        i+=35;j+=35;
		if(inside_area(970+5,970+45,MAXY-i,MAXY-j,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes2(390,415,colors[8]);
			boxes(355,380,colors[7]);
			boxes(320,345,colors[7]);
			boxes4(285,310,colors[8]);
			glColor3f(1,0.7,0.8);
			polygon(970+20,365,970+20,370,970+30,370,970+30,365);
			polygon(970+15,360-35,970+15,375-35,970+35,375-35,970+35,360-35);
			//polygon(970+10,355-70,970+10,380-70,970+40,380-70,970+40,355-70);

			//boxes(355,380,colors[7]);
			//boxes(320,345,colors[7]);
			//boxes(285,310,colors[7]);
			//boxes(250,275,colors[7]);
			glColor3f(0,0,0);
			draw_text("LARGE ERAZER",625,30);
			disp();
			glFlush();
			state1=ERAZE3;
			return;
		}
        ii+=35;jj+=35;
		if(inside_area(5,45,MAXY-ii,MAXY-jj,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			boxes3(355,380,colors[8]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			boxes1(215,240,colors[7]);
			glColor3f(0,0,0);
			draw_text("INSIDE CLIP - to remove parts inside a selected area",625,30);
			disp();
			glFlush();
			state1=INSIDECLIP;
			return;
		}
		ii+=35;jj+=35;
		if(inside_area(5,45,MAXY-ii,MAXY-jj,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			boxes1(355,380,colors[7]);
			boxes3(320,345,colors[8]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			boxes1(215,240,colors[7]);
			glColor3f(0,0,0);
			draw_text("OUTSIDE CLIP - to remove parts outside a selected area",625,30);
			disp();
			glFlush();
			state1=OUTSIDECLIP;
			return;
		}
		ii+=35;jj+=35;
		if(inside_area(5,45,MAXY-ii,MAXY-jj,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
			boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes3(285,310,colors[8]);
			boxes1(250,275,colors[7]);
			boxes1(215,240,colors[7]);
			glColor3f(0,0,0);
			draw_text("TRANSLATE - to shift selected area to desired location",625,30);
			disp();
			glFlush();
			state1=TRANSLATE;
			return;
		}
		ii+=35;jj+=35;
		if(inside_area(5,45,MAXY-ii,MAXY-jj,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
		    boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes3(250,275,colors[8]);
			boxes1(215,240,colors[7]);
			glColor3f(0,0,0);
			draw_text("SCALING - to zoom the selected area to twice its size",625,30);
			disp();
			glFlush();
			state1=SCALING;
			return;
		}
		ii+=35;jj+=35;
		if(inside_area(5,45,MAXY-ii,MAXY-jj,x,y))
		{
			glColor3fv(colors[17]);
			polygon(620,20,620,45,MAXX-5,45,MAXX-5,20);
		    boxes(635,660,colors[7]);
			boxes(600,625,colors[7]);
		    boxes(565,590,colors[7]);
			boxes(530,555,colors[7]);
			boxes(495,520,colors[7]);
			boxes(460,485,colors[7]);
			boxes(425,450,colors[7]);
			boxes(390,415,colors[7]);
			glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);
			boxes1(355,380,colors[7]);
			boxes1(320,345,colors[7]);
			boxes1(285,310,colors[7]);
			boxes1(250,275,colors[7]);
			boxes3(215,240,colors[8]);
			glColor3f(0,0,0);
			draw_text("ROTATION - Rotate a figure drawn",625,30);
			disp();
			glFlush();
			state1=ROTATION;
			return;
		}
		if(inside_area(5,45,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[8]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=NEW;
			return;		
		}
		if(inside_area(55,95,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[8]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=OPEN;
			openc=0;
			return;		
		}
		if(inside_area(105,145,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[8]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=SAVE;
			savec=0;
			return;		
		}
		if(inside_area(155,195,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[8]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=EXIT;
			return;		
		}
		if(inside_area(205,245,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[8]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=TRANSLATE;
			return;		
		}
        if(inside_area(255,295,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[8]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=TRANSLATE1;
			return;
		}

		if(inside_area(205,245,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[8]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=INSIDECLIP;
			return;		
		}
		if(inside_area(257,297,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[8]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=OUTSIDECLIP;
			return;	
		}
		if(inside_area(312,358,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[8]);
			menu(367,408,MAXY-30,MAXY-5,colors[17]);
			menu_disp();
			state1=TRANSLATE;
			return;		
		}
		if(inside_area(367,408,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[8]);
			menu_disp();
			state1=SCALING;
			return;		
		}
		if(inside_area(422,458,MAXY-30,MAXY-5,x,y))
		{
			menu(5,45,MAXY-30,MAXY-5,colors[17]);
			menu(55,95,MAXY-30,MAXY-5,colors[17]);
			menu(105,145,MAXY-30,MAXY-5,colors[17]);
			menu(155,195,MAXY-30,MAXY-5,colors[17]);
			menu(205,245,MAXY-30,MAXY-5,colors[17]);
			menu(257,297,MAXY-30,MAXY-5,colors[17]);
			menu(312,358,MAXY-30,MAXY-5,colors[17]);
			menu(367,408,MAXY-30,MAXY-5,colors[8]);
			menu_disp();
			state1=ROTATION;
			return;		
		}
		if(state1==SPIRAL)
		{
			spiral(x,y);
		}	
	}
	if(state1==NEW)
	{
		glColor3fv(colors[7]);
		polygon(50,50,MAXX-50,50,MAXX-50,MAXY-40,50,MAXY-40);
		glColor3fv(colors[0]);
	    lineloop(50,50,MAXX-50,50,MAXX-50,MAXY-40,50,MAXY-40);
		line(51,51,51,MAXY-39);
		line(51,MAXY-41,MAXX-51,MAXY-41);
		glColor3fv(colors[17]);
			polygon(970,280,970,385,970+45,385,970+45,280);

        glColor3fv(colors[17]);
        polygon(0,MAXY-40,0,MAXY,MAXX,MAXY,MAXX,MAXY-40);
	polygon(MAXX-50,50,MAXX,50,MAXX,MAXY-40,MAXX-50,MAXY-40); //right
	polygon(0,0,0,MAXY-20,50,MAXY-20,50,0);  //left
		menu(5,45,MAXY-30,MAXY-5,colors[17]);
		menu(55,95,MAXY-30,MAXY-5,colors[17]);
		menu(105,145,MAXY-30,MAXY-5,colors[17]);
		menu(155,195,MAXY-30,MAXY-5,colors[17]);
		menu_disp();
		glColor3fv(colors[7]);
		int x=40,y=65,i;
		for(i=0;i<8;i++)
		{    
			polygon(970+5,MAXY-x,970+5,MAXY-y,970+45,MAXY-y,970+45,MAXY-x);
			x+=35;
			y+=35;
		}
		for(i=0;i<4;i++)
		{    
			polygon(5,MAXY-x,5,MAXY-y,45,MAXY-y,45,MAXY-x);
			x+=35;
			y+=35;
		}
		disp();
	}
	if(state1==SAVE && savec==0)
	{
		char filename[30];
		savec=1;
		glReadPixels(50,50,MAXX-50,MAXY-40,GL_RGB,GL_FLOAT,mat);
		printf("Enter the filename to save : ");
		scanf("%s",filename);
		fp=fopen(filename,"w");			
		fwrite(mat,matsize,sizeof(GLubyte),fp);
		fclose(fp);
		printf("\nFILE SAVED\n");	
		menu(5,45,MAXY-30,MAXY-5,colors[17]);
		menu(55,95,MAXY-30,MAXY-5,colors[17]);
		menu(105,145,MAXY-30,MAXY-5,colors[16]);
		menu(155,195,MAXY-30,MAXY-5,colors[17]);
		menu_disp();
		glFlush();
	}
	if(state1==OPEN && openc==0)
	{
		char filename[30];
		openc=1;
		printf("\nEnter filename to open : ");
		scanf("%s",filename);
		fp=fopen(filename,"r");		
		fread(mat,matsize,sizeof(GLubyte),fp);
		fclose(fp);			
		glColor3fv(colors[7]);
		polygon(50,50,MAXX-50,50,MAXX-50,MAXY-40,50,MAXY-40);
		glFlush();
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-40,GL_RGB,GL_FLOAT,mat);
		printf("\nFILE OPENED\n");
		menu(5,45,MAXY-30,MAXY-5,colors[17]);
		menu(55,95,MAXY-30,MAXY-5,colors[17]);
		menu(105,145,MAXY-30,MAXY-5,colors[16]);
		menu(155,195,MAXY-30,MAXY-5,colors[17]);
		menu_disp();
		glFlush();
	}
	if(trans_paste==1)
	{ 
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		if(x>=50)			
			if(y>=50)			
				if((x+px-1)<=(MAXX-50))
					if((y+py-1)<=(MAXY-40))
					{
						glColor3fv(colors[7]);
						polygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
						glRasterPos2i(x,y);
						glDrawPixels(px-1,py-1,GL_RGB,GL_FLOAT,clip);		
					}
		glFlush();
		trans_paste=0;
	}

	if(trans_paste1==1)
	{ 
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		if(x>=50)			
			if(y>=50)			
				if((x+px-1)<=(MAXX-50))
					if((y+py-1)<=(MAXY-40))
					{
						//glColor3fv(colors[7]);
						//olygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
						glRasterPos2i(x,y);
						glDrawPixels(px-1,py-1,GL_RGB,GL_FLOAT,clip);		
					}
		glFlush();
		trans_paste1=0;
	}
	if(rotate_paste==1)
	{ 
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		if(x>=50)			
			if(y>=50)			
				if((x+px-1)<=(MAXX-50))
					if((y+py-1)<=(MAXY-40))
					{
						glPushMatrix();
						glTranslatef(x,y,0);
						glRotatef(30,0,0,1);
						glTranslatef(-x,-y,0);
						glBegin(GL_LINE_LOOP);
						glVertex2f(x,y);
						glVertex2f(x,y+py);
						glVertex2f(x+px,y+py);
						glVertex2f(x+px,y);
						glEnd();
						glFlush();
						glPopMatrix();
					}
		glFlush();
		rotate_paste=0;
	}
	if(scale_paste==1)
	{ 
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		if(x>=50)			
			if(y>=50)			
				if((x+px-1)<=(MAXX-50))
					if((y+py-1)<=(MAXY-40))
					{
						glColor3fv(colors[7]);
						polygon(ax,by,ax,by+py,ax+px,by+py,ax+px,by);
						glPixelZoom(2,2);
						glRasterPos2i(x,y);
						glDrawPixels(px-1,py-1,GL_RGB,GL_FLOAT,clip);		
					}
		glFlush();
		glPixelZoom(1,1);
		scale_paste=0;
	}
	if(state1==EXIT)
	{
		exit(0);
	}	
	if(state==GLUT_UP)
	{
		if(outsideclip)
		{
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney,px,py,GL_RGB,GL_FLOAT,clip);
			glColor3fv(colors[7]);
			polygon(50,50,MAXX-50,50,MAXX-50,MAXY-40,50,MAXY-40);
			glFlush();
			glRasterPos2i(linex,eliney);
			glDrawPixels(px,py,GL_RGB,GL_FLOAT,clip);
			outsideclip=0;
		}	
		if(insideclip)
		{
			glColor3fv(colors[7]);
			printf("%d %d %d %d",linex,liney,elinex,eliney);
			polygon(linex,liney,linex,eliney,elinex,eliney,elinex,liney);
			lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			glFlush();
			insideclip=0;
		}	

		if(translate)
		{
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);			
			trans_paste=1;
			translate=0;
		}		
		if(translate1)
		{
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);			
			trans_paste1=1;
			translate1=0;
		}
		if(rotate)
		{
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);
			printf("HERE");
			rotate_paste=1;
			glFlush();
			rotate=0;
		}
		if(scaling)
		{
			ax=linex;ay=liney;bx=elinex;by=eliney;
			px=elinex-linex; py=liney-eliney;
			glReadPixels(linex,eliney+1,px-1,py-1,GL_RGB,GL_FLOAT,clip);			
			scale_paste=1;
			scaling=0;
		}	
		if(drawline)
		{
			glBegin(GL_LINES);
			glVertex2i(linex,liney);
			glVertex2i(elinex,eliney);
			glEnd();
			glFlush();
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawline=0;
		}
		if(drawrect)
		{
			lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawrect=0;
		}
		if(drawcircle)
		{
			double r=sqrt(pow((double)(elinex-linex),2)+(pow((double)(eliney-liney),2)));
			if((linex-r)<50)
				r=linex-50;
			if((liney-r)<50)
				r=liney-50;
			if((linex+r)>(MAXX-51))
				r=MAXX-51-linex;
			if((liney+r)>(MAXY-41))
				r=MAXY-41-liney;
			circle(linex,liney,r);
			glFlush();
			linex=0;
			liney=0;
			elinex=0;
			eliney=0;
			drawcircle=0;	
		}
		glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr1);
	}
	
}

void mymove(int mx,int my)
{
	GLint x=mx;
	GLint y=MAXY-my;	
	if(state1==INSIDECLIP)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				insideclip=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[0]);
			}						
		}
	}
	if(state1==OUTSIDECLIP)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				outsideclip=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[0]);
			}						
		}
	}
	if(state1==TRANSLATE)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				translate=1;
				rotate=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[0]);
			}						
		}
	}

	if(state1==TRANSLATE1)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				translate1=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[0]);
			}						
		}
	}
	if(state1==SCALING)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				scaling=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[0]);
			}						
		}
	}
	if(state1==ROTATION)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				rotate=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
				glColor3fv(colors[0]);
			}						
		}
	}
	if(state1==BRUSH)
	{
		if(x<(MAXX-57) && x>49 && y>49 && y<(MAXY-47))
		{
			GLint brushsize=8;
			glRecti(x,y,x+brushsize,y+brushsize);
		}
		glFlush();
	}
	if(state1==PENCIL)
	{
		if(x<(MAXX-49) && x>49 && y>49 && y<(MAXY-39))
		{
			glBegin(GL_POLYGON);
			glVertex2f(x,y);
			glVertex2f(x,y+1.8);
			glVertex2f(x+1.8,y+1.8);
			glVertex2f(x+1.8,y);
			glEnd();			
			glFlush();
		}
	}
	if(state1==ERAZE1)
	{

		if(x<(MAXX-57) && x>49 && y>49 && y<(MAXY-47))
		{
			glColor3f(1.0,1.0,1.0);
			GLint brushsize=8;
			glRecti(x,y,x+brushsize,y+brushsize);
		}
		glFlush();
	}
	if(state1==ERAZE2)
	{

		if(x<(MAXX-57) && x>49 && y>49 && y<(MAXY-47))
		{
			glColor3f(1.0,1.0,1.0);
			GLint brushsize=16;
			glRecti(x,y,x+brushsize,y+brushsize);
		}
		glFlush();
	}
	if(state1==ERAZE3)
	{

		if(x<(MAXX-57) && x>49 && y>49 && y<(MAXY-47))
		{
			glColor3f(1.0,1.0,1.0);
			GLint brushsize=24;
			glRecti(x,y,x+brushsize,y+brushsize);
		}
		glFlush();
	}
	if(state1==SPRAY)
	{
		if(x<(MAXX-50) && x>59 && y>54 && y<(MAXY-45))
		{
			glPointSize(1.0);
			spray(x,y);
			glFlush();
		}
	}
	if(state1==LINE)
	{
		if(x<MAXX-49 && x>49 && y>49 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawline=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				glBegin(GL_LINES);
				glVertex2i(linex,liney);
				glVertex2i(x,y);
				glEnd();
				glFlush();
			}
		}
	}
	if(state1==RECT)
	{
		if(x<MAXX-49 && x>49 && y>50 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawrect=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				lineloop(linex,liney,linex-1,eliney,elinex,eliney,elinex,liney);
			}
		}
	}
	if(state1==FILLRECT)
	{
		if(x<MAXX-49 && x>49 && y>49 && y<MAXY-38)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}
			else
			{
				drawline=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				glBegin(GL_POLYGON);
				glVertex2i(linex,liney);
				glVertex2i(linex,eliney);
				glVertex2i(elinex,eliney);
				glVertex2i(elinex,liney);
				glEnd();
     			glFlush();
			}
		}
	}
	if(state1==CIRCLE)
	{
		if(x<MAXX-49 && x>49 && y>49 && y<MAXY-39)
		{
			if(!linex && !liney)
			{
				glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
				linex=x;
				liney=y;
			}	
			else
			{
				drawcircle=1;
				elinex=x;
				eliney=y;
				glRasterPos2i(50,50);
				glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr);
		   		double r=sqrt(pow((double)(elinex-linex),2)+(pow((double)(eliney-liney),2)));
				if((linex-r)<50)
					r=linex-50;
				if((liney-r)<50)
					r=liney-50;
				if((linex+r)>(MAXX-51))
					r=MAXX-51-linex;
				if((liney+r)>(MAXY-41))
					r=MAXY-41-liney;
				circle(linex,liney,r);				
				glFlush();			
			}
		}
	}	
}

void display()
{
	glReadPixels(50,50,MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr1);
	glClear(GL_COLOR_BUFFER_BIT);	
	glColor3fv(colors[17]);
	polygon(0,MAXY-40,0,MAXY,MAXX,MAXY,MAXX,MAXY-40); //top
	menu_disp();
    glColor3fv(colors[17]);
	polygon(MAXX-50,50,MAXX,50,MAXX,MAXY-40,MAXX-50,MAXY-40); //right
	polygon(0,0,0,MAXY-20,50,MAXY-20,50,0);  //left
	polygon(0,0,0,50,MAXX,50,MAXX,0); //bottom
	glColor3fv(colors[7]);
	polygon(50,5,50,25,75,25,75,5);
	glColor3fv(colors[0]);
	polygon(200+50,5,200+50,25,200+75,25,200+75,5);
	glColor3fv(colors[16]);
	polygon(75,5,75,25,100,25,100,5);
	glColor3fv(colors[10]);
	polygon(200+75,5,200+75,25,200+100,25,200+100,5);
	glColor3fv(colors[6]);
	polygon(100,5,100,25,125,25,125,5);
	glColor3fv(colors[3]);
	polygon(200+100,5,200+100,25,200+125,25,200+125,5);
	glColor3fv(colors[2]);
	polygon(125,5,125,25,150,25,150,5);
	glColor3fv(colors[11]);
	polygon(200+125,5,200+125,25,200+150,25,200+150,5);
	glColor3fv(colors[13]);
	polygon(150,5,150,25,175,25,175,5);
	glColor3fv(colors[9]);
	polygon(200+150,5,200+150,25,200+175,25,200+175,5);
	glColor3fv(colors[15]);
	polygon(175,5,175,25,200,25,200,5);
	glColor3fv(colors[14]);
	polygon(200+175,5,200+175,25,200+200,25,200+200,5);
	glColor3fv(colors[5]);
	polygon(200,5,200,25,225,25,225,5);
	glColor3fv(colors[1]);
	polygon(200+200,5,200+200,25,200+225,25,200+225,5);
	glColor3fv(colors[4]);
	polygon(225,5,225,25,250,25,250,5);
	glColor3fv(colors[12]);
	polygon(200+225,5,200+225,25,200+250,25,200+250,5);
	glColor3fv(colors[7]);
	glPointSize(2.0);
	lineloop(50,5,50,25,450,25,450,5);
	line(50,5,50,25);
	line(75,5,75,25);
	line(100,5,100,25);
	line(125,5,125,25);
	line(150,5,150,25);
	line(175,5,175,25);
	line(200,5,200,25);
	line(225,5,225,25);
	line(250,5,250,25);
	line(275,5,275,25);
	line(300,5,300,25);
	line(325,5,325,25);
	line(350,5,350,25);
	line(375,5,375,25);
	line(400,5,400,25);
	line(425,5,425,25);
	line(450,5,450,25);
	glColor3fv(colors[7]);
	int x=40,y=65,i;
	for(i=0;i<8;i++)
	{    
		polygon(970+5,MAXY-x,970+5,MAXY-y,970+45,MAXY-y,970+45,MAXY-x);
		x+=35;
		y+=35;
	}
	for(i=0;i<6;i++)
	{    
		polygon(5,MAXY-x,5,MAXY-y,45,MAXY-y,45,MAXY-x);
		x+=35;
		y+=35;
	}
	disp();
	glFlush();
	glColor3fv(colors[0]);
	polygon(10,10,10,40,40,40,40,10);
	lineloop(9,10,10,40,40,40,40,10);
	glColor3fv(colors[7]);
	polygon(50,50,MAXX-50,50,MAXX-50,MAXY-40,50,MAXY-40);
	glColor3fv(colors[0]);
	glPointSize(2.0);
	lineloop(50,50,MAXX-50,50,MAXX-50,MAXY-40,50,MAXY-40);
	if(count!=0)
	{
		glRasterPos2i(50,50);
		glDrawPixels(MAXX-50,MAXY-50,GL_RGB,GL_FLOAT,arr1);
	}
	else count++;
	draw_text(" ",MAXX-500,MAXY-10);
	glFlush();
}

void keyboard(unsigned char keyPressed, int x, int y)
{	
	if(keyPressed == '+') size ++;
	if(keyPressed == '-') size --;
}


void myReshape(int newWidth,int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, MAXX, 0.0,MAXY);
	glColor3f(0.0, 0.0, 0.0);
}


void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(MAXX, MAXY);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Graphical Editor");	
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mymove);
	glutMainLoop();
}