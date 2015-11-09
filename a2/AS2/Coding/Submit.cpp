// Ass2.cpp : Defines the entry point for the console application.
// Author: YE ZHIZHEN
// SID: 1155046993

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
#include <cmath>

#define FRONT_RADIUS 0.05
#define BACK_RADIUS 0.13
#define PI 3.14159265358979323846
#define DISPLACEMENT 0.1

#define FRICTION 1.0
#define MOVEFRONT	1
#define MOVEBACK	0
#define STATIC		2
#define INITIAL_Z 0.4
#define WHEEL_ANGLE 10
#define POWER 2.6
#define PLANE 7.0
#define BRAKE 3.6
#define NORMALVIEW 0

float winwidth, winheight;
//x,-4+y,5+z
//x,y,z
//float cam_X(0), cam_Y(-4), cam_Z(5);
float cam_X(0), cam_Y(0), cam_Z(5);
float cam_ViewX(0), cam_ViewY(0), cam_ViewZ(0);
//position of car
float velocity(0.0),acc(-FRICTION);
float velocity_X(0),velocity_Y(0);
float position_X(0),position_Y(0),position_Z(INITIAL_Z);
//angle of wheel
float cons_rotate_front(0),cons_rotate_back(0);
float velocity_angle(0.0);
float wheelangle(0.0);
int viewstate(NORMALVIEW);

GLubyte* groundImage;
GLuint groundTex;

int groundTex_w;
int groundTex_h;

GLubyte* carImage;
GLuint carTex;

int carTex_w;
int carTex_h;

GLubyte* wheelImage;
GLuint wheelTex;
GLUquadricObj *cylinder;

int wheelTex_w;
int wheelTex_h;
int direction = MOVEFRONT;

/* Add code here to define variable  
*/

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
	//glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0,0.3,0.6,0.6);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	// Texture mapping setting for Microsoft's OpenGL implementation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);


	/* Add code here to initialize lighting, read bitmap images, initialize different GLU geometry
	* use glLight, TextureLoadBitmap, gluNewQuadric...etc
	*/
	

	//light 0
	//position
	 //GLfloat light0pos[] = {-1.f, -1.f, 1.f, 0.f};
	 GLfloat light0pos[] = {0.f, 0.f, 1.f, 1.f};
	//ambient
	 GLfloat light0_emb[] = {0.1, 0.1, 0.1, 1.f};
	//diffuse
	 GLfloat light0_diff1[] = {1.0, 1.0, 1.0, 1.f};

	

	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_emb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diff1);
	

	
	//glEnable(GL_LIGHT2);
	//Init ground texture
	groundImage = TextureLoadBitmap("kawori.bmp",&groundTex_w,&groundTex_h); 
	glGenTextures(1,&groundTex);
	glBindTexture(GL_TEXTURE_2D,groundTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//Assign the image to a texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, groundTex_w, 
				groundTex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, 
				groundImage);
	
	carImage = TextureLoadBitmap("car.bmp", &carTex_w, &carTex_h);
	glGenTextures(1,&carTex);
	glBindTexture(GL_TEXTURE_2D,carTex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
				   GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				   GL_NEAREST);
	
	//Assign the image to a texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, carTex_w, 
				carTex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, 
				carImage);

	//init texture to wheel
	wheelImage = TextureLoadBitmap("wheel.bmp", &wheelTex_w, &wheelTex_h);
	glGenTextures(1,&wheelTex);
	glBindTexture(GL_TEXTURE_2D,wheelTex);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
				   GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				   GL_NEAREST);
	
	//Assign the image to a texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wheelTex_w, 
				wheelTex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, 
				wheelImage);
	// init quadric object
	cylinder = gluNewQuadric();
	gluQuadricTexture(cylinder, GL_TRUE);
	//light1,car light
	double inten_mat = 0.9*4;
	double inten_diff = 0.9*4;
	
	static GLfloat light1_mat[] = {inten_mat, inten_mat, 0, 0.9f };
	//static GLfloat light1_diff[] = {inten_diff, inten_diff, 0, 0.9f };

	
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);


	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_mat);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.f); 
	glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,100);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}



void drawParticle(float v)
{
	static float max_distance = 0.9; 
	static float distance;
	static float width;
	static int pieces;
	static float xs;
	pieces = 20;
	//glPushMatrix();
	//glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION );
	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
	glEnable ( GL_COLOR_MATERIAL );
	glColor3f(0.1,0.4,0.7);
	//glMaterialfv(GL_FRONT,GL_DIFFUSE, particle_diffuse);
	for(int i=0; i<pieces;i++)
	{
		distance = (float)i/pieces * max_distance;	
		width = 0.15 + distance/1.732 *2;
		//generate (1-distance/max_Distance)^2 *velocity in the width
		for(int k=1; k< pow((1-distance/max_distance),3)*pow(v+2,2)/10.f; k++)
		{	
			
			//between -width/2 ~ width/2
			xs = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/width))-width/2;
			glPushMatrix();
				glTranslatef(xs,-distance,0);
				
				gluSphere(cylinder,0.01,5,5);
			glPopMatrix();
		}
	}
	glColor3f(1.0,1.0,1.0);
	glDisable( GL_COLOR_MATERIAL);
	//glPopMatrix();
}




void drawCar(void)
{
	
	static float max_distance = 0.9; 
	static float distance;
	static float width;
	static int pieces;
	static float xs;
	pieces = 20;
	static GLfloat car_diffuse[] = {0.6, 0.6, 0.6, 1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, car_diffuse);
	//glLightf(GL_FRONT,GL_SPOT_CUTOFF,45.f);
	glPushMatrix();
		//first, draw the body
		glTranslatef(position_X,position_Y,position_Z);
		glScalef(1.0,2.0,1.0);
		//rotate to right orientation
		glRotatef(velocity_angle, 0.f, 0.f, -1.f); 
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,carTex);
		glRotatef(90, -1.0, 0, 0);		
		gluCylinder(cylinder,0.15,0.1,0.3,30,30);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		if(velocity>0)
			drawParticle(velocity);
		//four wheels
		
		glPushMatrix();
		//right front
		glTranslatef(2.2*FRONT_RADIUS,0.28,0.1);
		glRotatef(90.f, 0.0, 1.0, 0.0);
		glColor3f(0.45,0.45,0.45);
		//rotate when press left or right
		glRotatef(wheelangle,1.0, 0, 0);
		//constant rotate
		glRotatef(-cons_rotate_front,0,0,1);
		
		//axis
		gluCylinder(cylinder,FRONT_RADIUS,FRONT_RADIUS,0.05,10,10);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, wheelTex);
		gluDisk(cylinder,0,FRONT_RADIUS,10,10);
		glTranslatef(0, 0, 0.05);
		gluDisk(cylinder,0,FRONT_RADIUS,10,10);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glPushMatrix();
		//left front
		glTranslatef(-3.1*FRONT_RADIUS,0.28,0.1);
		glRotatef(90.f, 0.0, 1.0, 0.0);
		glColor3f(0.45,0.45,0.45);
		//rotate when press left or right
		glRotatef(wheelangle,1.0, 0, 0);
		//constant rotate
		glRotatef(-cons_rotate_front,0,0,1);
		//axis
		gluCylinder(cylinder,FRONT_RADIUS,FRONT_RADIUS,0.05,10,10);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, wheelTex);
		gluDisk(cylinder,0,FRONT_RADIUS,10,10);
		glTranslatef(0, 0, 0.05);
		gluDisk(cylinder,0,FRONT_RADIUS,10,10);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//left back
		glPushMatrix();
		glTranslatef(-BACK_RADIUS*1.3,0.0,0.1);
		glRotatef(90.f, 0.0, 1.0, 0.0);
		glColor3f(0.45,0.45,0.45);
		glRotatef(-cons_rotate_back,0,0,1);
		//axis
		gluCylinder(cylinder,BACK_RADIUS,BACK_RADIUS,0.05,10,10);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, wheelTex);
		gluDisk(cylinder,0,BACK_RADIUS,10,10);
		glTranslatef(0, 0, 0.05);
		gluDisk(cylinder,0,BACK_RADIUS,10,10);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		//right back
		glPushMatrix();
		glTranslatef(BACK_RADIUS*1.1,0.0,0.1);
		glRotatef(90.f, 0.0, 1.0, 0.0);
		glColor3f(0.45,0.45,0.45);
		glRotatef(-cons_rotate_back,0,0,1);
		//axis
		gluCylinder(cylinder,BACK_RADIUS,BACK_RADIUS,0.05,10,10);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		//wheel
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, wheelTex);
		gluDisk(cylinder,0,BACK_RADIUS,10,10);
		glTranslatef(0, 0, 0.05);
		gluDisk(cylinder,0,BACK_RADIUS,10,10);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();
}

void drawGround(void)
{
	glPushMatrix();
	//for plane
	static GLfloat plane_diffuse[] = {0.7, 0.7, 0.7, 1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, plane_diffuse);
	static float step;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,groundTex);
	step = 0.08;
	//gluSphere(sphere, 3.0, 20, 20);
	glBegin(GL_QUADS); 
	for(float a=-PLANE;a<PLANE;a+=step)
	{
		for(float b=-PLANE;b<PLANE;b+=step)
		{
		 
		glNormal3f(0,0,1);
		glTexCoord3f(a, b, 0.0);	glVertex3f (a, b, 0.0);
		glNormal3f(0,0,1);
		glTexCoord3f(a, (b+step), 0.0);glVertex3f (a, (b+step), 0.0);
		glNormal3f(0,0,1);
		glTexCoord3f((a+step), (b+step), 0.0);glVertex3f ((a+step), (b+step), 0.0);
		glNormal3f(0,0,1);
		glTexCoord3f((a+step), b, 0.0);	glVertex3f ((a+step), b, 0.0);
		
		}	
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void display(void)
{
	
	/* clear the window color before drawing is performed */
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	static float upx;
	static float upy;
	static int upz;
	switch(viewstate)
	{
		case NORMALVIEW:
			upx=0;	upy=1;	upz=0;
			break;
		case NORMALVIEW+1:
			upx=1;	upy=0;	upz=0;
			break;
		case NORMALVIEW+2:
			upx=0;
			//printf("%d\n",upx);
			//printf("%d\n",upy);
			upy=0;
			upz=1;
			break;
	}
	//printf("%f\n",velocity_angle);
	gluLookAt(cam_X,cam_Y,cam_Z, cam_ViewX, cam_ViewY, cam_ViewZ, upx, upy, upz);
	/* Add code here to transform the car and the ground & draw them
	* use glTranslate, glRotate, glLoadIdentity, glPushMatrix, glPopMatrix, glMaterial, 
	* glBegin, gluSphere...etc
	*
	* Add code for Texture Mapping for the car and the ground
	* use glTexImage2D..
	*/
		//update car light
	static GLfloat light1pos[4];
	light1pos[0]= position_X+0.66*sin(velocity_angle*PI/180);
	light1pos[1]= position_Y+0.66*cos(velocity_angle*PI/180);
	light1pos[2]= position_Z-0.3;
	light1pos[3]= 1.f;

	static GLfloat light1direction[3];
	light1direction[0] = sin(velocity_angle*PI/180);
	light1direction[1] = cos(velocity_angle*PI/180);
	light1direction[2] = 0;
	//printf("light1position[0]:%f,light1position[1]:%f\n",light1pos[0],light1pos[1]);
	/*light1direction[0] = 0;
	light1direction[1] = 0;
	light1direction[2] = 0.5;*/
	//light1direction[3] = 1;
	//printf("light1direction[0]:%f,light1direction[1]:%f\n",light1direction[0],light1direction[1]);
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,light1direction);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	drawGround();
	drawCar();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();

}

void reshape(GLsizei w,GLsizei h)
{
	glMatrixMode(GL_PROJECTION);
	winwidth  = w;
	winheight = h;
	glViewport(0, 0, w, h);	
	glLoadIdentity();
	gluPerspective(45,(GLfloat)w/(GLfloat)h,0.9,40.0); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* keyboard up CALLBACK for handling special keyboard up event */
void keyspecialup(int key, int x, int y)
{

	/* Add code here to control the animation interactively */

	switch (key)
	{
		// car control
		case GLUT_KEY_UP: // move front
			acc = -FRICTION;
			break;
		case GLUT_KEY_DOWN: // move back
			acc = -FRICTION;
			break;
		case GLUT_KEY_LEFT: // move left
			wheelangle = 0;
			break;
		case GLUT_KEY_RIGHT: // move right
			wheelangle = 0;
			break;
	}
}



/* keyboard CALLBACK for handling special keyboard event */
void keyspecial(int key, int x, int y)
{

	/* Add code here to control the animation interactively */
	switch (key)
	{
		// car control
		case GLUT_KEY_LEFT:
			wheelangle = -WHEEL_ANGLE;
			if(direction != STATIC)
				velocity_angle -= WHEEL_ANGLE*0.35;
			break;
		case GLUT_KEY_UP:
			//MOVEFRONT;
			if(direction==MOVEFRONT || direction == STATIC)
			{
				acc = POWER - FRICTION;
				direction = MOVEFRONT;
			}
			else if(direction == MOVEBACK)
			{
				acc = -BRAKE - FRICTION;
			}
			break;
		case GLUT_KEY_DOWN:
			//MOVEBACK;
			if(direction == MOVEFRONT)
				acc = -FRICTION - BRAKE;
			else if(direction == MOVEBACK || direction == STATIC)
			{
				acc = POWER - FRICTION;
				direction = MOVEBACK;
			}	
			break;
		case GLUT_KEY_RIGHT:
			if(direction != STATIC)
				velocity_angle += WHEEL_ANGLE*0.35;
			wheelangle = WHEEL_ANGLE;
			break;
	}
}

/* keyboard CALLBACK for handling keyboard event */
void keyboard(unsigned char key, int x, int y)
{
	/* Add code here to control the animation interactively */

	switch (key)
	{
		case 'r': // Reset all parameters
			velocity = 0;
			acc = -FRICTION;
			position_X = 0.f;
			position_Y = 0.f;
			position_Z = INITIAL_Z;
			cons_rotate_front = 0.f;
			cons_rotate_back = 0.f;
			velocity_angle = 0.f;
			direction = STATIC;
			break;
		case '1':
			cam_Z -= DISPLACEMENT;
			break;
		case '2':
			cam_Z += DISPLACEMENT;
			break;
		case 'w':
			position_Z += DISPLACEMENT*5;
			break;
		case 's':
			position_Z -= DISPLACEMENT*5;
			break;	
		case 'k':
			velocity_angle += 20.0;
			break;
		case 27: //esc pressed
			exit(0);
			break;
		case '3':
			viewstate = (viewstate+1)%3;
			break;
	}

}

void mouse(int button, int state, int ix, int iy)
{
	
}

void drag(int ix, int iy)
{

}

/* Add code here to update the velocity, acceleration, position and rotation of car and wheels */
// display after update and reset timer
void update(int value)
{
	static float time = value /1000.f;
	static float lightangle;
	//printf("time :%f\n",time);
	static float displacement;
	static float X_component;
	static float Y_component;
	float X_component_back;
	float Y_component_back;
	static GLfloat light0pos[4];
	lightangle += 2;
	lightangle = std::fmod(lightangle,360);
	light0pos[0] = -1 + cos(lightangle*PI/180);
	light0pos[1] = -1 + sin(lightangle*PI/180);
	light0pos[2] = 1;
	light0pos[3] = 0;
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);

	

	if(velocity>=0&&direction!=STATIC)
	{
		//displacement should always be positive
		displacement = velocity*time + 1/2.f*acc*time*time;
		
		velocity += acc*time;
		if(direction==MOVEFRONT)
		{
			Y_component = displacement*cos(velocity_angle/180*PI);
			X_component = displacement*sin(velocity_angle/180*PI);	
			if((position_X<=PLANE && position_X>=-PLANE)||(X_component>0 && position_X <-PLANE)||(X_component<0 && position_X> PLANE))		
				position_X += X_component;
				//printf("X_component:%f,position_X:%f\n",X_component,position_X);
			
			
				//printf("position x:%f\n",position_X);
			
			if((position_Y<=PLANE && position_Y>=-PLANE)||(position_Y>PLANE && Y_component<0)||(position_Y<-PLANE && Y_component>0))
				position_Y += Y_component;

			
			//printf("position y:%f\n",position_Y);
			cons_rotate_front += displacement/FRONT_RADIUS*180/PI;
			cons_rotate_front = std::fmod(cons_rotate_front,360);
			cons_rotate_back += displacement/BACK_RADIUS*180/PI;
			cons_rotate_back = std::fmod(cons_rotate_back,360);
		}
		else if(direction==MOVEBACK)
		{

			X_component_back = displacement*-sin(velocity_angle/180*PI);
			Y_component_back = displacement*-cos(velocity_angle/180*PI);
			
			if((position_X<=PLANE && position_X>=-PLANE)||(X_component_back>0 && position_X <-PLANE)||(X_component_back<0 && position_X> PLANE))
				position_X += X_component_back;
			
			if((position_Y<=PLANE && position_Y>=-PLANE)||(position_Y>PLANE && Y_component_back<0)||(position_Y<-PLANE && Y_component_back>0))
				position_Y += Y_component_back;

			cons_rotate_front -= displacement/FRONT_RADIUS*180/PI;
			cons_rotate_front = std::fmod(cons_rotate_front,360);
			cons_rotate_back -= displacement/BACK_RADIUS*180/PI;
			cons_rotate_back = std::fmod(cons_rotate_back,360);
		}
	}
	if(velocity<0)
	{
		velocity = 0;
		direction = STATIC;
	}
	if(viewstate==NORMALVIEW || viewstate == NORMALVIEW+1)
	{
		cam_X = position_X;
		cam_Y = -4+position_Y;
		cam_ViewX = position_X;
		cam_ViewY = position_Y;
		cam_Z = 5;
	}
	else
	{
		cam_X = position_X - 0.2*sin(velocity_angle/180*PI);
		cam_Y = position_Y - 0.2*cos(velocity_angle/180*PI);
		cam_Z = position_Z;
		cam_ViewX = position_X + 3*sin(velocity_angle/180*PI);
		cam_ViewY = position_Y + 3*cos(velocity_angle/180*PI);
		cam_ViewZ = position_Z+0.3;
	}
	glutPostRedisplay();
	glutTimerFunc(10,update,10);
}

//void idle

void main(int argc, char** argv)
{

	/*Initialization of GLUT Library */
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	/*Create a window with title specified */
	glutCreateWindow("Assignment 2");

	/*Register different CALLBACK function for GLUT to response 
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	init();	/*not GLUT call, initialize several parameters */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyspecial);
	glutSpecialUpFunc(keyspecialup);
	// call timer function every 20 ms
	glutTimerFunc(10, update, 10);
	//glutMouseFunc(mouse);
	glutMotionFunc(drag);
	//glutIdleFunc(idle);
	
	srand (time(NULL));
	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
	
}


