
//#include "stdafx.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "bitmap.h"

float winwidth, winheight;

/* Add code here to define variable  
*/

GLubyte* sphereImage;
GLuint sphereTex;

int sphereTex_w;
int sphereTex_h;

int angle_x = 0;
int angle_y = 0;

GLUquadricObj *sphere;

//Load the BMP file
GLubyte* TextureLoadBitmap(char *filename, int *w, int *h)		/* I - Bitmap file to load */
{
	BITMAPINFO	*info;				/* Bitmap information */
	void		*bits;				/* Bitmap pixel bits */
	GLubyte	*rgb;				/* Bitmap RGB pixels */
	GLubyte   err = '0';

	/*
	* Try loading the bitmap and converting it to RGB...
	*/

	bits = LoadDIBitmap(filename, &info);
	if(bits==NULL) 
		return(NULL);
	rgb = ConvertRGB(info, bits);
	if (rgb == NULL)
	{
		free(info);
		free(bits);
	};

	printf("%s: %d %d\n", filename, info->bmiHeader.biWidth, info->bmiHeader.biHeight);
	printf("read %s successfully\n", filename);
	*w = info->bmiHeader.biWidth;
	*h = info->bmiHeader.biHeight;

	/*
	* Free the bitmap and RGB images, then return 0 (no errors).
	*/

	free(info);
	free(bits);
	return (rgb);

}


void init()
{
	// Clear Screen color
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0,0,0,0);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Texture mapping setting for Microsoft's OpenGL implementation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	// Texture mapping parameters for filter and repeatance
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/* Add code here to initialize lighting, read bitmap images, initialize different GLU spheres
	* and the position of the stars.
	* use glLight, TextureLoadBitmap, gluNewQuadric...etc
	*/
	GLfloat light_position[] = { 0.0, 5.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// init texture
	sphereImage = TextureLoadBitmap("sphere.bmp",&sphereTex_w,&sphereTex_h);
//-------------------------------------------------Important
	glGenTextures(1, &sphereTex);
	glBindTexture(GL_TEXTURE_2D, sphereTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
				   GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				   GL_NEAREST);
	//Assign the image to a texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sphereTex_w, 
				sphereTex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, 
				sphereImage);

	// init quadric object
	sphere = gluNewQuadric();
	gluQuadricTexture(sphere, GL_TRUE);
}

void display(void)
{

	/* clear the window color before drawing is performed */
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	/* Add code here to transform the planets and other objects & draw them, draw the star-sphere by points, 
	* material properties of the Sun & planets should be set correctly.
	* use glTranslate, glRotate, glLoadIdentity, glPushMatrix, glPopMatrix, glMaterial, 
	* glBegin, gluSphere...etc
	*
	* Add code for Texture Mapping for each planets and objects
	* use glTexImage2D..
	*/
	glLoadIdentity();		
	gluLookAt( 0.0, 3.0,3.0, 0, 0.0, 0, 0.0, 1.0, 0.0);

	glPushMatrix();
		glRotatef(angle_x,1,0,0);
		glRotatef(angle_y,0,1,0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, sphereTex);
		gluSphere(sphere, 0.2, 20, 20);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glFlush();

	glutSwapBuffers();

}

void reshape(GLsizei w,GLsizei h)
{
	winwidth  = w;
	winheight = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(GLfloat)w/(GLfloat)h,1.0,40.0); 

	glMatrixMode(GL_MODELVIEW);	
}

/* keyboard up CALLBACK for handling special keyboard event */
void keyspecialup(int key, int x, int y)
{

	/* Add code here to control the animation interactively */

	switch (key)
	{
		case GLUT_KEY_UP: 
			break;
		case GLUT_KEY_DOWN: 
			break;
		case GLUT_KEY_LEFT: 
			break;
		case GLUT_KEY_RIGHT: 
			break;
	}
}

/* keyboard CALLBACK for handling special keyboard event */
void keyspecial(int key, int x, int y)
{

	/* Add code here to control the animation interactively */

	switch (key)
	{
		case GLUT_KEY_UP: 
			if (angle_x == 0){
				angle_x = 360;
			}
			angle_x -= 10;
			break;
		case GLUT_KEY_DOWN:	
			if (angle_x == 360){
				angle_x = 0;
			}
			angle_x += 10;
			break;
		case GLUT_KEY_LEFT: 
			if (angle_y == 0){
				angle_y = 360;
			}
			angle_y -= 10;
			break;
		case GLUT_KEY_RIGHT: 
			if (angle_y == 360){
				angle_y = 0;
			}
			angle_y += 10;
			break;
	}
}

/* keyboard CALLBACK for handling keyboard event */
void keyboard(unsigned char key, int x, int y)
{
	/* Add code here to control the animation interactively */

	switch (key)
	{
		// Reset all parameters
		case 'r':	angle_x = 0;
					angle_y = 0;
					break;
	}

}

void mouse(int button, int state, int ix, int iy)
{
	
}




void drag(int ix, int iy)
{

}

void timer(int t)
{
	glutPostRedisplay();
    glutTimerFunc(10, timer, 1);
}

void main(int argc, char** argv)
{

	/*Initialization of GLUT Library */
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	/*Create a window with title specified */
	glutCreateWindow("Texture Demo Program");

	/*Register different CALLBACK function for GLUT to response 
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyspecial);
	//glutSpecialUpFunc(keyspecialup);
	glutTimerFunc(10, timer, 1);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	init();	/*not GLUT call, initialize several parameters */

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
	
	gluDeleteQuadric(sphere);
}


