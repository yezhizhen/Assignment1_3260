//global variable
int fanAngle = 0;
int fanSpeed = 1;

GLUquadricObj *DiskObj;
GLUquadricObj *DiskObj1;

void drawWindHouse(void){

DiskObj = gluNewQuadric(); //New an object
DiskObj1 = gluNewQuadric(); //New an object

glColor4f(0.9, 0.8, 1, 1); //rice white
glTranslatef(0, 50, 0);
gluCylinder(DiskObj, 9, 7, 40, 8, 20);
glTranslatef(0, 0, 40);
gluDisk(DiskObj1, 0, 7, 8, 20 );

glColor4f(0.2, 0.1, 0, 1); //dark grey
glutSolidSphere(6, 20, 20);

glTranslatef(0, -7, 0);
glutSolidSphere(2, 20, 20);

glPushMatrix();
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
		glColor4f(0.9, 0.8, 1, 1);
		glScalef(20, 40, 20);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix(); // draw the door
	glColor4f(0.2, 0.1, 0, 1);
	glTranslatef(0, -16, 3);
	glutSolidCube(10);
	glPopMatrix();

	glPushMatrix(); // draw the window
	glColor4f(0.22, 0.22, 0.22, 1);
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
	glColor4f(0.2, 0.1, 0, 1);
	glScalef(15, 29, 15);
	glRotated(45, 0, 0, -1);
	glutSolidOctahedron();

glPopMatrix();

}
