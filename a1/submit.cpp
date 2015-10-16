/*****************************************************************************
        FILE : submit.cpp (csci3260 2015-2016 Assignment 1)
        NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
		Student Information
		Student ID: 1155046993
		Student Name: YE ZHIZHEN
		DATE: 11/10/2015
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <gl\glut.h>
#include <gl\glu.h>
#include <gl\gl.h>
#include <math.h>
#include <time.h>
void reshape(int w, int h);

int winWidth = 800;
int winHeight = 550;

float cam_X(0), cam_Y(0), cam_Z(5);
float cam_ViewX(0), cam_ViewY(250), cam_ViewZ(-300);

#define DISPLACEMENT 0.1
#define MOVEUP cam_Y+=DISPLACEMENT;cam_ViewY+=DISPLACEMENT;
#define MOVEDOWN cam_Y-=DISPLACEMENT;cam_ViewY-=DISPLACEMENT;
#define MOVELEFT cam_X-=DISPLACEMENT;cam_ViewX-=DISPLACEMENT;
#define MOVERIGHT cam_X+=DISPLACEMENT;cam_ViewX+=DISPLACEMENT;

GLUquadric* qobj;

float angle = 0.0;
float angle1 =0.3;
float angle3 = 0;
void keyboard(unsigned char key, int x, int y);

void init(void) // All Setup For OpenGL Goes Here
{
	// Light 0 Settings
	//position
	static GLfloat light0pos[] = {1.f, 1.f, 1.f, 0.f};
	//ambient
	static GLfloat light0_emb[] = {0.2, 0.2, 0.2, 1.f};
	//diffuse
	static GLfloat light0_diff1[] = {1.0, 1.0, 1.0, 1.f};
	//specular
	static GLfloat light0_specular[] = {0.5,0.5,0.5,1.f};
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_emb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	//for second
	//position
	static GLfloat light1pos[] = {-1.f, -1.f, 1.f, 0.f};
	//ambient
	static GLfloat light1_emb[] = {0.2, 0.2, 0.2, 1.f};
	//diffuse
	static GLfloat light1_diff1[] = {0.8, 0.8, 0.8, 1.f};
	//specular
	static GLfloat light1_specular[] = {0.5,0.5,0.5,1.f};

	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_emb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diff1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	//for third
	//position
	static GLfloat light2pos[] = {0.5f, -2.0f, 1.f, 1.f};
	//ambient
	static GLfloat light2_emb[] = {0.1, 0.1, 0.1, 1.f};
	//diffuse
	static GLfloat light2_diff1[] = {0.4, 0.4, 0.4, 1.f};
	//specular
	static GLfloat light2_specular[] = {0.2,0.2,0.2,1.f};

	glLightfv(GL_LIGHT2, GL_POSITION, light2pos);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_emb);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diff1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);		// Cull all the back face
	//glFrontFace(GL_CCW);		// Define Counter-Clockwise as front face
	//glCullFace(GL_BACK);
	qobj = gluNewQuadric();
}

void drawGround(void)
{
	glPushMatrix();
	//for plane
	static GLfloat plane_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, plane_diffuse);
	glBegin(GL_QUADS);  
	  glVertex3f (-4.0, -4.0, 0.0);
	  
	  glVertex3f (4.0, -4., 0.0);
	 
	  glVertex3f (4.0, 4.0, 0.0);

	  glVertex3f (-4.0, 4.0, 0.0);
	glEnd();

	glPopMatrix();
}



GLfloat sun_diffuse[] = {0.9, 0.3, 0.0, 1.0};
GLfloat x = 2.5;
int flag2=0;
float year=0.0;
float day=0.0;
void drawSun(void)
{	
	glPushMatrix();
	static GLfloat high_shininess[] = {100.0};
	static GLfloat mat_sunspecular[] = {0.2, 0.4, 0.0, 1.0};

	glMaterialfv(GL_FRONT,GL_DIFFUSE, sun_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR, mat_sunspecular);
	glMaterialfv(GL_FRONT,GL_SHININESS, high_shininess);
	
	glTranslatef(x,2.5,3);
	glRotatef(angle,0,1,0);
	glutSolidSphere(0.5,80,20);
	glRotatef(year,0.0,1.0,0.0);
	glTranslatef(1.0,0.0,0.0);
	glRotatef(day, 0.0, 1.0, 0.0);
	glutSolidSphere(0.3,40,20);
	
	glPopMatrix();

}



void treeAni(int a)
{
	if(flag2 == 0) angle3+=2.0;
	else angle3-=2.0;
	if(angle3>=20)	flag2 = 1;
	if(angle3<=-20)	flag2 = 0;
	glutTimerFunc(100,treeAni,-1);
}

void drawTrees(void)
{
	glPushMatrix();
	static GLfloat root_diffuse[] = {0.4, 0.2, 0.0, 1.0};
	static GLfloat root_specular[] = {0.1, 0.3, 0.1, 1.0};
	static GLfloat tree_diffuse[] = {0.0, 0.8, 0.0, 1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,root_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,root_specular);
	glTranslatef(-2.0,-1.0,1.0);
	gluCylinder(qobj, 0.2, 0.2, 0.61, 100, 80);
	//at cylinder;
	glPushMatrix();
	glTranslatef(0.0,0.0,0.4);

	glMaterialfv(GL_FRONT,GL_DIFFUSE,tree_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,root_specular);
	
	glRotatef(angle3,1,0,0);
	glutSolidCone(0.3,1.2,20,20);
	glRotatef(-angle3,1,0,0);
	
	glTranslatef(-0.7,-0.2,0.0);
	glRotatef(angle3,1,0,0);
	glutSolidCone(0.3,1.2,20,20);
	glRotatef(-angle3,1,0,0);
	//at second cone
	glPushMatrix();
	glTranslatef(-0.3,-0.7,0.0);
	glRotatef(angle3,1,0,0);
	glutSolidCone(0.3,1.2,20,20);
	//back to second cone
	glPopMatrix();
	//back to first cylinder
	glPopMatrix();
	glMaterialfv(GL_FRONT,GL_DIFFUSE,root_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,root_specular);
	glTranslatef(-0.7,-0.2,0.0);
	gluCylinder(qobj, 0.2, 0.2, 0.61, 100, 80);
	glTranslatef(-0.3,-0.7,0.0);
	gluCylinder(qobj, 0.2, 0.2, 0.61, 100, 80);	
	glPopMatrix();
}

void drawDesk(void)
{
	//-0.25,-0.5
	glPushMatrix();
	static GLfloat desk_diffuse[] = {0.2, 0.2, 0.0, 1.0};
	static GLfloat desk_sunspecular[] = {0.2, 0.0, 0.5, 1.0};
	static GLfloat board_diffuse[] = {1.0,1.0,0.23, 1.0};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,desk_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,desk_sunspecular);
	//Draw a Table
	//first leg
	glTranslatef(-0.5,-3.0,0);
	glScalef(0.25,0.25,2);
	glutSolidCube(1);
	glPopMatrix();
	//second leg
	glPushMatrix();
	glMaterialfv(GL_FRONT,GL_DIFFUSE,desk_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,desk_sunspecular);
	glTranslatef(1.0,-3.0,0.0);
	glScalef(0.25,0.25,2);
	glutSolidCube(1);
	glPopMatrix();

	//third
	glPushMatrix();
	glMaterialfv(GL_FRONT,GL_DIFFUSE,desk_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,desk_sunspecular);
	glTranslatef(1.00,-2.5,0.0);
	glScalef(0.25,0.25,2);
	glutSolidCube(1);
	glPopMatrix();
	
	//fourth
	glPushMatrix();
	glMaterialfv(GL_FRONT,GL_DIFFUSE,desk_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,desk_sunspecular);
	glTranslatef(-0.5,-2.5,0.0);
	glScalef(0.25,0.25,2);
	glutSolidCube(1);
	glPopMatrix();

	//board
	glPushMatrix();
	glMaterialfv(GL_FRONT,GL_DIFFUSE,board_diffuse);
	glTranslatef(0.25,-2.75,1.2);
	glScalef(2.0,1.0,0.3);
	glutSolidCube(1);
	glPushMatrix();
	//teapot
	static GLfloat teapot_diffuse[]={0.4,0.2,0.0,1};
	static GLfloat teapot_specular[]={0.1,0.1,0.1,1};
	glTranslatef(-0.25,0.1,1.5);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,teapot_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,teapot_specular);
	glRotatef(50,0.2,0.3,0.1);
	glutSolidTeapot(0.22);
	glPopMatrix();
	//back to board, draw a electric fan
	static GLfloat fan_diffuse[]={0.23, 0.23, 0.23, 1.0};
	static GLfloat fan_specular[]={0.3, 0.3, 1.0, 1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,fan_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,fan_specular);
	glTranslatef(0.25,-0.1,1.0);
	//glutSolidTorus(0.03,0.1,30,30);
	gluCylinder(qobj, 0.2, 0.11, 3.21, 100, 80);
	glTranslatef(0.0,0.0,3.4);
	glRotatef(angle1,1,
		0,0);
	gluDisk(qobj,0.05,0.25,20,20);
	glPopMatrix();

}

void drawHouse(void)
{
	glPushMatrix();
	static GLfloat house_shininess[] = {5.0};
	static GLfloat house_diffuse[] = {0.6,0.6,0.2,1.0};
	static GLfloat house_specular[] = {0.3, 0.3, 0.3, 1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, house_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR, house_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS, house_shininess);
	glTranslatef(1.0,0.0,1.0);
	glutSolidCube(2.0);
	//center of cube
	glPushMatrix();
	//draw door
	static GLfloat door_diffuse[] = {0.5,0.25,0.0,1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, door_diffuse);
	
	glBegin(GL_QUADS);
		glVertex3f(-0.3,-1.0,-1);
		glVertex3f(0.75,-1.0,-1);
		glVertex3f(0.75,-1.0,0.3);
		glVertex3f(-0.3,-1.0,0.3);
	glEnd();
	static GLfloat window_diffuse[] = {0.13, 0.8, 0.8, 1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, window_diffuse);
	glBegin(GL_QUADS);
		glVertex3f(-0.8,-1.0,0.5);
		glVertex3f(-0.6,-1.0,0.5);
		glVertex3f(-0.6,-1.0,0.8);
		glVertex3f(-0.8,-1.0,0.8);
	glEnd();
	glPopMatrix();
	//back to center
	static GLfloat roof_diffuse[] = {0.2, 0.3, 0.3, 1.0};
	glTranslatef(0.0,0.0,1.5);
	glMaterialfv(GL_FRONT,GL_DIFFUSE, roof_diffuse);
	glutSolidCone(1.3,0.6,40,20);
	glTranslatef(-0.8,-0.8,-0.5);
	//remember first root
	glPushMatrix();
	glScalef(0.5,0.5,2.00);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.6,0,0);
	glScalef(0.5,0.5,2.00);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,1.6,0);
	glScalef(0.5,0.5,2.00);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.6,1.6,0);
	glScalef(0.5,0.5,2.00);
	glutSolidCube(0.5);
	glPopMatrix();
	glPopMatrix();
}

void display(void) // Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(cam_X,cam_Y,cam_Z, cam_ViewX, cam_ViewY, cam_ViewZ, 0, 1, 1);
	drawGround();
	drawSun();
	drawDesk();
	drawTrees();
	drawHouse();
	glPopMatrix();
	glutSwapBuffers();
	glFlush();	
	glutPostRedisplay();

}


void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{

	winWidth = w; winHeight = h;
	
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, (float)w/(float)h, 2, 50); 
	//near, far is the distance to the camera
	//glOrtho(-10.0,10.0,-10.0,10.0,2.0,100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void special(int key, int x, int y) // Handle special keys
{	
	switch (key) 
	{
		case GLUT_KEY_LEFT:
			MOVELEFT;
			break;
		case GLUT_KEY_UP:
			MOVEUP;
			break;
		case GLUT_KEY_DOWN:
			MOVEDOWN;
			break;
		case GLUT_KEY_RIGHT:
			MOVERIGHT;
			break;
	}
}

void menuclick(int value){
	switch(value){
		case 0:
			printf("Full screen entry selected!\n");
			keyboard('f', 0, 0);
			break;
		case 1:
			glutDetachMenu(GLUT_RIGHT_BUTTON);
			break;
	}

}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{	
	printf("pressed key: %d\n",key);
	static bool fulls = false;
	switch (key) 
	{	
		case 27://press 'esc' to quit
		   exit(0);
		   break;
		case 't':
			printf("Close the light1!\n");
			glDisable(GL_LIGHT1);
			break;
		case 'y':
			printf("Close the light2!\n");
			glDisable(GL_LIGHT2);
			break;
		case 'u':
			printf("Close the light2!\n");
			glEnable(GL_LIGHT2);
			break;
		case 'o':
			printf("Open the light1!\n");
			glEnable(GL_LIGHT1);
			break;
		case 'f':
			if(!fulls)
			{
			glutFullScreen();
			fulls = true;
			}
			else
			{
				glutPositionWindow(100,20);
				glutReshapeWindow(600,600);
				fulls = false;
			}
			break;
		case 'w':
			cam_ViewY+= DISPLACEMENT*100;
			break;
		case 's':
			cam_ViewY-= DISPLACEMENT*100;
			break;
		case 'a':
			cam_ViewX-= DISPLACEMENT*100;
			break;
		case 'd':
			cam_ViewX+= DISPLACEMENT*100;
			break;
		case '1':
			cam_Z-=DISPLACEMENT;
			break;
		case '2':
			cam_Z+=DISPLACEMENT;
			break;
	}
}

void mouseclick(int button, int state, int x, int y) // Handle the mouse click events here
{

	printf("button: %d, state: %d",button,state);
	
}

//winWidth,winHeight
void mousemove(int x, int y) // Handle the mouse movement events here
{
	float x_lef = 1.0/8*winWidth;
	float x_rig	= 7.0/8*winWidth;
	float y_low	= 1.0/8 * winHeight;
	float y_high = 7.0/8 * winHeight;
	if(x< x_lef && y < y_low)
	{
		MOVELEFT;
		MOVEUP;
	}
	else if(x<x_lef && y>  y_high)
	{
		MOVELEFT;
		MOVEDOWN;
	}
	else if(x<x_lef && y<=y_high && y>= y_low)
	{
		MOVELEFT;
	}
	else if(x>x_rig&& y<=y_low)
	{	
		MOVERIGHT;
		MOVEUP;
	}
	else if(x>x_rig&& y>= y_high)
	{	
		MOVERIGHT;
		MOVEDOWN;
	}
	else if(x>x_rig&& y<=y_high && y>=y_low)
	{
		MOVERIGHT;
	}
	else if(x>x_lef&&x<x_rig&&y>=y_high)
	{
		MOVEDOWN;
	}
	else if(x>x_lef&&x<x_rig&&y<=y_low)
	{
		MOVEUP;
	}
}


void idle()
{
	Sleep(10);
}


//add the menu here
void setMenu(){
	int menuidentifier = glutCreateMenu(menuclick);
	glutAddMenuEntry("full screen",0);
	glutAddMenuEntry("Detach the mouse",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int flag=0;
int flag1=0;

void diskspin(int)
{	
	if(flag1 == 0) angle1+=5.0;
	else angle1-=5.0;
	if(angle1>=70)	flag1 = 1;
	if(angle1<=-50)	flag1 = 0;
	glutTimerFunc(200,diskspin,-1);
}
void sunSpin(int)
{	
	sun_diffuse[0]+=0.05;
	if(sun_diffuse[0] >=1.0)
	{
		sun_diffuse[0]=0;
	}
	sun_diffuse[1]+=0.05;
	if(sun_diffuse[1] >=1.0)
	{
		sun_diffuse[1]=0.0;
	}
	angle+=4.0;
	if(flag == 0)	x-=0.1; 
	else x += 0.1;
	
	if(x<=-2.5)	flag = 1;
	if(x>=2.5)	flag = 0;
	glutPostRedisplay();
	glutTimerFunc(200,sunSpin,-1);

}
void main(int argc, char** argv)
{
	/*Initialization of GLUT Library */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	/*Create a window with title specified */
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(50, 0);
	//return value is identifier for the window
	int windownumber = glutCreateWindow("Assignment 1");
	//printf("window number: %d\n",windownumber);
	init();	/*not GLUT call, initialize several parameters */
	
	/*Register different CALLBACK function for GLUT to response 
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutPassiveMotionFunc(mousemove);
	glutMouseFunc(mouseclick);
	setMenu();
	
	//the idle callback is continuously called when events are not being received
	glutIdleFunc(idle);
	diskspin(-1);
	sunSpin(-1);
	treeAni(-1);
	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	//glutFullScreen();
	glutMainLoop();
	
}