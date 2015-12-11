//////////////////////////////////////////////////////////////////////////
//Please fill your name and student ID
//Name: YE ZHIZHEN      SONG YANG
//StuID: 1155046993		1155046870
//////////////////////////////////////////////////////////////////////////
#include "MyObjects.h"
//include opengl
#ifndef MY_OB_JECT
#define MY_OB_JECT
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#define DISPLACEMENT_Y 0.5  
#define WIDTH 0.5
#endif // !MY_OB_JECT

GLUquadricObj *DiskObj= gluNewQuadric();
GLUquadricObj *DiskObj1= gluNewQuadric();
void MyObjects::drawTrees(TrainView* tv,bool doingShadows)
{
	glPushMatrix();
	glTranslated(-90, 0, 0);
	if (!doingShadows) glColor3f(1, 0.647, 0);

	glPushMatrix();
	glRotatef(-90,1,0,0);
	
	gluCylinder(DiskObj,1,1,17,10,5);

	glPopMatrix();
	if (!doingShadows) glColor3f(0.180, 0.545, 0.341);
	glRotatef(-90,1,0,0);
	glTranslatef(0,0,8.5);
	gluCylinder(DiskObj,5,0,15,20,10);
	/*for (float j = 0; j < 2.5 * 3.14; j += 0.01f)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 10, 0);
		glVertex3f(10 * cos(j), 10, 10 * sin(j));
		glVertex3f(0, 30, 0);
		glEnd();
	}*/
	glPopMatrix();

	glPushMatrix();
	glTranslated(90, 0, 0);
	if (!doingShadows) glColor3f(1, 0.647, 0);
	
	glPushMatrix();
	glRotatef(-90,1,0,0);
	
	gluCylinder(DiskObj,1,1,17,10,5);

	glPopMatrix();
	if (!doingShadows) glColor3f(0.180, 0.545, 0.341);
	glRotatef(-90,1,0,0);
	glTranslatef(0,0,8.5);
	gluCylinder(DiskObj,5,0,15,20,10);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, -90);
	if (!doingShadows) glColor3f(1, 0.647, 0);
	
	glPushMatrix();
	glRotatef(-90,1,0,0);
	
	gluCylinder(DiskObj,1,1,17,10,5);

	glPopMatrix();
	if (!doingShadows) glColor3f(0.180, 0.545, 0.341);
	glRotatef(-90,1,0,0);
	glTranslatef(0,0,8.5);
	gluCylinder(DiskObj,5,0,15,20,10);
	glPopMatrix();
	glPopMatrix();
}

void MyObjects::drawCarriage(TrainView* tv,bool doingShadows)
{
	glPushMatrix();
	glScalef(0.38,0.55,0.8);
	glTranslatef(0, 0, -5);
	//for the body part
	glBegin(GL_QUADS);
	// front
	//(x-axis, z-axis, y-axis)
	if (!doingShadows) glColor3f(0.153f, 0.619f, 0.035f);
	glVertex3f(0, 4, 0);
	glVertex3f(0, 14, 0);
	glVertex3f(0, 14, 10);
	glVertex3f(0, 4, 10);

	// back
	if (!doingShadows) glColor3f(0.668f, 0.219f, 0.035f);
	glVertex3f(30, 4, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 14, 10);
	glVertex3f(30, 4, 10);

	// top
	if (!doingShadows) glColor3f(0.367f, 0.761f, 0.937f);
	glVertex3f(0, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 14, 10);
	glVertex3f(0, 14, 10);

	// bottom
	if (!doingShadows) glColor3f(0.467f, 0.761f, 0.937f);
	glVertex3f(0, 4, 0);
	glVertex3f(30, 4, 0);
	glVertex3f(30, 4, 10);
	glVertex3f(0, 4, 10);

	// left
	if (!doingShadows) glColor3f(0.165f, 0.580f, 0.102f);
	glVertex3f(0, 4, 0);
	glVertex3f(0, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 4, 0);

	// right
	if (!doingShadows) glColor3f(0.265f, 0.380f, 0.102f);
	glVertex3f(0, 4, 10);
	glVertex3f(0, 14, 10);
	glVertex3f(30, 14, 10);
	glVertex3f(30, 4, 10);
	glEnd();

	//for the wheels part
	//left and front wheel
	glPushMatrix();
	glTranslated(7.5, 4, 10);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.15f)
	{
		for (float j = 0; j < 8 * 3.14; j += 0.05f)
		{
			glBegin(GL_QUADS);
			if (!doingShadows) glColor3f(0, 0, 0);
			glVertex3f(cos(j), sin(j), i);
			glVertex3f(cos(j + 1), sin(j + 1), i);
			glVertex3f(cos(j + 1), sin(j + 1), i + 1);
			glVertex3f(cos(j), sin(j), i + 1);
			glEnd();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(7.5, 4, 12);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + 0.05), sin(k + 0.05), 0);
		glEnd();
	}
	glPopMatrix();

	//left and back wheel
	glPushMatrix();
	glTranslated(22.5, 4, 10);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.15f)
	{
		for (float j = 0; j < 8 * 3.14; j += 0.05f)
		{
			glBegin(GL_QUADS);
			if (!doingShadows) glColor3f(0.0, 0.0, 0.0);
			glVertex3f(cos(j), sin(j), i);
			glVertex3f(cos(j + 1), sin(j + 1), i);
			glVertex3f(cos(j + 1), sin(j + 1), i + 1);
			glVertex3f(cos(j), sin(j), i + 1);
			glEnd();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(22.5, 4, 12);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + 0.05), sin(k + 0.05), 0);
		glEnd();
	}
	glPopMatrix();

	//right and front wheel
	glPushMatrix();
	glTranslated(7.5, 4, -2);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.15f)
	{
		for (float j = 0; j < 8 * 3.14; j += 0.05f)
		{
			glBegin(GL_QUADS);
			if (!doingShadows) glColor3f(0, 0, 0);
			glVertex3f(cos(j), sin(j), i);
			glVertex3f(cos(j + 1), sin(j + 1), i);
			glVertex3f(cos(j + 1), sin(j + 1), i + 1);
			glVertex3f(cos(j), sin(j), i + 1);
			glEnd();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(7.5, 4, -2);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + 0.05), sin(k + 0.05), 0);
		glEnd();
	}
	glPopMatrix();
	//right and back wheel
	glPushMatrix();
	glTranslated(22.5, 4, -2);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.15f)
	{
		for (float j = 0; j < 8 * 3.14; j += 0.05f)
		{
			glBegin(GL_QUADS);
			if (!doingShadows) glColor3f(0, 0, 0);
			glVertex3f(cos(j), sin(j), i);
			glVertex3f(cos(j + 1), sin(j + 1), i);
			glVertex3f(cos(j + 1), sin(j + 1), i + 1);
			glVertex3f(cos(j), sin(j), i + 1);
			glEnd();
		}
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(22.5, 4, -2);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += 0.05)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + 0.05), sin(k + 0.05), 0);
		glEnd();
	}
	glPopMatrix();

	glPopMatrix();
}


float height=20.1;
float y_direction=1;
float x_direction=1;
int time = 0;
float z_direction=1;
float x_pos = 50;
float z_pos = -50;
float x_dis = 0;
float z_dis = 0;
void MyObjects::drawBallon(TrainView* tv, bool doingShadows)
{
	static GLfloat setback[] ={0,0,0,1};
	static GLfloat ballon_spec[] = {0.2,0.4,0.2,1.0};
	if(height>80)
	{
		y_direction = -1;
	}
	else if(height<20)
	{
		y_direction = 1;
	}
	///generate 0 to 1
	if((time == 0)|| (time++>500) )
	{
		time = 1;
		x_dis = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/WIDTH)) - WIDTH/2;
		z_dis = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX/WIDTH)) - WIDTH/2;
	}
	x_pos += x_dis;
	z_pos += z_dis;
	if(x_pos > 120)	x_dis = -0.25;
	if(x_pos < -120) x_dis = 0.25;
	if(z_pos > 120)	 z_dis = -0.25;
	if(z_pos < -120) z_dis = 0.25;
	glPushMatrix();
	glTranslatef(x_pos,height,z_pos);
	glScalef(1,0.6,0.9);
		if(!doingShadows)
		{
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,ballon_spec);
			glColor3f(0.807,0.733,0.051);
		}
		glutSolidCube(6);
		glTranslatef(0,3,0);
		glLineWidth(2);
		if(!doingShadows)	glColor3f(0.4, 1, 0.4);
		glBegin(GL_LINES);
		glVertex3f(3.0,0,3.0);
		glVertex3f(3.0,5.0,3.4);

		glVertex3f(3.0,0,-3.0);
		glVertex3f(3.0,5.0,-3.4);
		
		glVertex3f(-3.0,0,3.0);
		glVertex3f(-3.0,5.0,3.4);

		glVertex3f(-3.0,0,-3.0);
		glVertex3f(-3.0,5.0,-3.4);
		glEnd();
		glTranslatef(0,18,0);
		//Then, draw the ballon
		if(!doingShadows)	glColor3f(0.051, 0.631, 0.8078);
		glScalef(1.4,3,1.4);
		glutSolidSphere(5,30,10);
	glPopMatrix();
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, setback);

}

//global variable
int fanAngle = 0;
int fanSpeed = 1;



void MyObjects::drawWindHouse(TrainView* tv, bool doingShadows)
{

	fanAngle +=0.5;

	glPushMatrix();
	glTranslatef(-80, 10,-50);
	glRotatef(-90,1,0,0);
	
	if(!doingShadows) glColor4f(0.9, 0.8, 1, 1); //rice white
	glTranslatef(0, 50, 0);

	glPushMatrix();
	glTranslatef(0,0,-10);
	gluCylinder(DiskObj, 9, 7, 40, 8, 20);
	glPopMatrix();
	glTranslatef(0, 0, 40);
	glPushMatrix();
	glTranslatef(0,0,-10);
	gluDisk(DiskObj1, 0, 7, 8, 20 );
	
	if(!doingShadows) glColor4f(0.2, 0.1, 0, 1); //dark grey
	glutSolidSphere(6, 20, 20);
	glPopMatrix();
	glTranslatef(0, -7, 0);
	glPushMatrix();
	glTranslatef(0,0,-10);
	glutSolidSphere(2, 20, 20);
	glPopMatrix();
	glPushMatrix();

		glTranslatef(0,0,-10);
		if (fanAngle >= 360){ //rotate the fan
			fanAngle = 0;
		}
		else{
			fanAngle = fanAngle + fanSpeed;
		}
		glRotated(fanAngle, 0, 1, 0);

		glTranslatef(12, -1, 0); //draw the fan
		glPushMatrix();
		glScalef(20, 1, 5);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(-24, 0, 0);
		glPushMatrix();
		glScalef(20, 1, 5);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(12, 0, 12);
		glPushMatrix();
		glScalef(5, 1, 20);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0, 0, -24);
		glPushMatrix();
		glScalef(5, 1, 20);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0, 0, 12);
	glPopMatrix();

	glTranslatef(0, -42, -40);// after draw the fan, back to center point

	glPushMatrix();
		glTranslatef(20, 10, 0); //begin to draw the house
		glRotated(45, 0, 0, -1);

		glPushMatrix();
		if(!doingShadows) 	glColor4f(0.9, 0.8, 1, 1);
			glScalef(20, 40, 20);
			glutSolidCube(1);
		glPopMatrix();

		glPushMatrix(); // draw the door
		if(!doingShadows) glColor4f(0.2, 0.1, 0, 1);
		glTranslatef(0, -16, 3);
		glutSolidCube(10);
		glPopMatrix();

		glPushMatrix(); // draw the window
		if(!doingShadows) glColor4f(0.22, 0.22, 0.22, 1);
		glTranslatef(10, -10, 5);
		glPushMatrix();
		glScalef(2, 10, 5);
		glutSolidCube(1);
		glPopMatrix();
		glTranslatef(0, 20, 0);
		glScalef(2, 10, 5);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0, 0, 10); //draw the roof
		if(!doingShadows)  glColor4f(0.2, 0.1, 0, 1);
		glScalef(15, 29, 15);
		glRotated(45, 0, 0, -1);
		glutSolidOctahedron();

	glPopMatrix();
	glPopMatrix();

}
