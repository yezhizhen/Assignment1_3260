/*****************************************************************************
        FILE : submit.cpp (csci3260 2015-2016 Assignment 1)
        NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
		Student Information
		Student ID: 1155046993
		Student Name: YE ZHIZHEN
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <gl\glut.h>
#include <windows.h>
#include <math.h>
#include <time.h>

int winWidth = 600;
int winHeight = 600;

float cam_X(0), cam_Y(0), cam_Z(-250);
float cam_ViewX(0), cam_ViewY(0), cam_ViewZ(-500);


void init(void) // All Setup For OpenGL Goes Here
{
	// Light 0 Settings
	static GLfloat light0pos[] = {200.f, 100.f, 400.f, 0.f};
	static GLfloat light0_mat1[] = {0.8, 0.8, 0.8, 1.f};
	static GLfloat light0_diff1[] = {0.9, 0.9, 0.9, 1.f};
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_mat1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);		// Cull all the back face
	glFrontFace(GL_CCW);		// Define Counter-Clockwise as front face

	glEnable(GL_COLOR_MATERIAL); 
}



void display(void) // Here's Where We Do All The Drawing
{

	glClearColor(0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
		glVertex2f(-0.5,-1);
		glVertex2f(0.5,0.0);
		glVertex2f(0.0,0.0);
	glEnd();
	
	
	glBegin(GL_POLYGON);
      glVertex3f (0.25, 0.25, 0.0);
      glVertex3f (0.75, 0.25, 0.0);
      glVertex3f (0.75, 0.75, 0.0);
      glVertex3f (0.25, 0.75, 0.0);
   glEnd();
   
   glutSwapBuffers();
	//gluLookAt(cam_X,cam_Y,cam_Z, cam_ViewX, cam_ViewY, cam_ViewZ, 0, 1, 0);
	//glTranslatef(0, 0, -550);
	
	
	
		// A ball  
		// Remove this part and start to add your own drawing

		//End of A ball  


	// TODO:
	// Draw grounds and objects here
	






	//glutSwapBuffers();
	//glFlush();	


	// Refresh the frame
	glutPostRedisplay();
}


void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
	winWidth = w; winHeight = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, (float)w/(float)h, 250, 900); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void special(int key, int x, int y) // Handle special keys
{	
	switch (key) 
	{
		case GLUT_KEY_LEFT:
			printf("LEFT pressed");
			break;
		case GLUT_KEY_UP:
			break;
		case GLUT_KEY_DOWN:
			break;
		case GLUT_KEY_RIGHT:
			break;
	}
}

void menuclick(int value){
	switch(value){
		case 0:
			printf("Full screen entry selected!\n");
			glutFullScreen();
			break;
		case 1:
			glutDetachMenu(GLUT_RIGHT_BUTTON);
			break;
	}

}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{	
	printf("pressed key: %d\n",key);
	switch (key) 
	{
		case 'a':
			printf("a is pressed\n");
			break;
		
		case 27://press 'esc' to quit
		   exit(0);
		   break;
	//	case 'p':
	//		glutPushWindow();
	//		break;
	//	case 'o':
	//		glutPopWindow();
	//		break;
	//	case 'f':
	//		glutFullScreen();
	//		break;
		//	case '1':
	//		glutDestroyWindow(glutGetWindow());
	   /*
		   // TODO:
		   // Add keyboard control here
		case '':
			break;
		case '':
			break;
		case '':
			break;
		case '':
			break;	

	}*/
	}
}

void mouseclick(int button, int state, int x, int y) // Handle the mouse click events here
{
	printf("Button %d, State %d, Position %d, %d \n", button, state, x, y);
}

void mousemove(int x, int y) // Handle the mouse movement events here
{
	printf("Mouse moved to position %d %d \n", x, y);	
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
void main(int argc, char** argv)
{

	/*Initialization of GLUT Library */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	/*Create a window with title specified */
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	//return value is identifier for the window
	int windownumber = glutCreateWindow("Assignment 1");
	//printf("window number: %d\n",windownumber);
	init();	/*not GLUT call, initialize several parameters */
	
	/*Register different CALLBACK function for GLUT to response 
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	//glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutPassiveMotionFunc(mousemove);
	glutMouseFunc(mouseclick);
	setMenu();
	
	//the idle callback is continuously called when events are not being received
	glutIdleFunc(idle);
	
	
	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	//glutFullScreen();
	glutMainLoop();
	
}