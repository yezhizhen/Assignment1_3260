//  Train Project
// TrainView class implementation
// see the header for details
// look for TODO: to see things you want to add/change
// 

//////////////////////////////////////////////////////////////////////////
//Please fill your name and student ID
//Name: YE ZHIZHEN
//StuID: 1155046993
//////////////////////////////////////////////////////////////////////////

#include "TrainView.H"
#include "TrainWindow.H"
#include "MyObjects.h"

#include "Utilities/3DUtils.H"

#include <Fl/fl.h>
#include <time.h>
#include "Utilities/bitmap.h"
#include <GL/glut.h>
#define PI 3.14159265358979f

// we will need OpenGL, and OpenGL needs windows.h
#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include <math.h>

GLUquadricObj *cylinder;
GLfloat colors[12][3]=				// Rainbow Of Colors
{
	{1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
	{0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
	{0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};




typedef struct						// Create A Structure For Particle
{
	bool	active;					// Active (Yes/No)
	float	life;					// Particle Life
	float	fade;					// Fade Speed
	float	r;					// Red Value
	float	g;					// Green Value
	float	b;					// Blue Value
	float	x;					// X Position
	float	y;					// Y Position
	float	z;					// Z Position
	float	xi;					// X Direction
	float	yi;					// Y Direction
	float	zi;					// Z Direction
	float	xg;					// X Gravity
	float	yg;					// Y Gravity
	float	zg;					// Z Gravity
}particles;
particles particle[MAX_NUM];

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


TrainView::TrainView(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x,y,w,h,l)
{

	srand(time(NULL));
	cylinder = gluNewQuadric();
	for(int loop=0;loop<MAX_NUM;loop++)					// Initials All The Textures
	{
		particle[loop].active=true;					// Make All The Particles Active
		particle[loop].life=1.0f;					// Give All The Particles Full Life
		particle[loop].fade=float(rand()%100)/1000.0f+0.003f;		// Random Fade Speed
		particle[loop].r=colors[(loop+1)/(MAX_NUM/12)][0];	// Select Red Rainbow Color
		particle[loop].g=colors[(loop+1)/(MAX_NUM/12)][1];	// Select Green Rainbow Color
		particle[loop].b=colors[(loop+1)/(MAX_NUM/12)][2];	// Select Blue Rainbow Color
		particle[loop].x = 0;					// X Position
		particle[loop].y = 5.f;					// Y Position
		particle[loop].z = 0;					// Z Position
		particle[loop].xi=-100.f+ float((rand()%200));		// Random Speed On X Axis
		particle[loop].yi= float((rand()%550)) + 50;		// Random Speed On Y Axis
		particle[loop].zi= -100.f+float((rand()%200));		// Random Speed On Z Axis
		particle[loop].xg=0.0f;						// Set Horizontal Pull To Zero
		particle[loop].yg=-4.2f;					// Set Vertical Pull Downward
		particle[loop].zg=0.0f;						// Set Pull On Z Axis To Zero
	}
	for(int i=0;i<MAX_CAR_NUM ;i++)
	{
		this->newcars[i].o_angle = 0;
	}
	mode( FL_RGB|FL_ALPHA|FL_DOUBLE | FL_STENCIL );
	resetArcball();
}

void TrainView::resetArcball()
{
	// set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this,40,250,.2f,.4f,0);
}

// FlTk Event handler for the window
// TODO: if you want to make the train respond to other events 
// (like key presses), you might want to hack this.
int TrainView::handle(int event)
{
	// see if the ArcBall will handle the event - if it does, then we're done
	// note: the arcball only gets the event if we're in world view
	if (tw->worldCam->value())
		if (arcball.handle(event)) return 1;

	// remember what button was used
	static int last_push;

	switch(event) {
		case FL_PUSH:
			last_push = Fl::event_button();
			if (last_push == 1) {
				doPick();
				damage(1);
				return 1;
			};
			break;
		case FL_RELEASE:
			damage(1);
			last_push=0;
			return 1;
		case FL_DRAG:
			if ((last_push == 1) && (selectedCube >=0)) {
				ControlPoint &cp = world->points[selectedCube];

				double r1x, r1y, r1z, r2x, r2y, r2z;
				getMouseLine(r1x,r1y,r1z, r2x,r2y,r2z);

				double rx, ry, rz;
				mousePoleGo(r1x,r1y,r1z, r2x,r2y,r2z, 
						  static_cast<double>(cp.pos.x), 
						  static_cast<double>(cp.pos.y),
						  static_cast<double>(cp.pos.z),
						  rx, ry, rz,
						  (Fl::event_state() & FL_CTRL) != 0);
				cp.pos.x = (float) rx;
				cp.pos.y = (float) ry;
				cp.pos.z = (float) rz;
				damage(1);
			}
			break;
			// in order to get keyboard events, we need to accept focus
		case FL_FOCUS:
			return 1;
		case FL_ENTER:	// every time the mouse enters this window, aggressively take focus
				focus(this);
				break;
		case FL_KEYBOARD:
		 		int k = Fl::event_key();
				int ks = Fl::event_state();
				if (k=='p') {
					if (selectedCube >=0) 
						printf("Selected(%d) (%g %g %g) (%g %g %g)\n",selectedCube,
							world->points[selectedCube].pos.x,world->points[selectedCube].pos.y,world->points[selectedCube].pos.z,
							world->points[selectedCube].orient.x,world->points[selectedCube].orient.y,world->points[selectedCube].orient.z);
					else
						printf("Nothing Selected\n");
					return 1;
				};
				if(k == 'u'){
					if (selectedCube >=0) {
						world->points[selectedCube].pos.y++;
						this->tw->damageMe();
					}else
						printf("Nothing Selected\n");
					return 1;
				}
				break;
	}
	return Fl_Gl_Window::handle(event);
}

int flag = 0;
GLfloat fogColor[4] = {0.5, 0.7, 0.9, 0.5}; 
GLfloat density; 
// this is the code that actually draws the window
// it puts a lot of the work into other routines to simplify things
void TrainView::draw()
{
	density = this->tw->FOG_SLIDER->value();
	
	glViewport(0,0,w(),h());

	// clear the window, be sure to clear the Z-Buffer too
	if(this->tw->Fog->value())
		glClearColor(0.5f, 0.7f, 0.9f, 0.5f);
	else  glClearColor(0,0,.3f,0);		
		
	// we need to clear out the stencil buffer since we'll use
	// it for shadows
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Texture mapping setting for Microsoft's OpenGL implementation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);


	//you want to load bitmap
	if(!flag)
	{
		loadTex(0,"kawori.bmp");

		loadTex(1,"cars.bmp");

		loadTex(2,"top.bmp");
		
		loadTex(3,"cars4.bmp");

		flag = 1;
	}

	// Blayne prefers GL_DIFFUSE
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();		// put the code to set up matrices here

	// TODO: you might want to set the lighting up differently
	// if you do, 
	// we need to set up the lights AFTER setting up the projection

	// enable the lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// top view only needs one light
	if (tw->topCam->value()) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	} else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}
	// set the light parameters
	GLfloat lightPosition1[] = {0,1,1,0}; // {50, 200.0, 50, 1.0};
	GLfloat lightPosition2[] = {1, 0, 0, 0};
	GLfloat lightPosition3[] = {0, -1, 0, 0};
	GLfloat yellowLight[] = {0.5f, 0.5f, .1f, 1.0};
	GLfloat whiteLight[] = {1.0f, 1.0f, 1.0f, 1.0};
	GLfloat blueLight[] = {.1f,.1f,.3f,1.0};
	GLfloat grayLight[] = {.3f, .3f, .3f, 1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);
	


	glMatrixMode(GL_MODELVIEW);
	if(this->tw->Fog->value())
	{
		glEnable(GL_FOG);
		glFogi(GL_FOG_MODE, GL_EXP2);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_DENSITY, density/1000);
		
		//glHint(GL_FOG_HINT, GL_DONT_CARE);
		//glFogf(GL_FOG_START, 1.0);
		//glFogf(GL_FOG_END, 5.0);
	}
	else
	{
		glDisable(GL_FOG);
	}
	// now draw the ground plane
	setupFloor();
	glDisable(GL_LIGHTING);
	drawFloor(200,1);
	glEnable(GL_LIGHTING);
	setupObjects();
	// we draw everything twice - once for real, and then once for
	// shadows
	drawStuff();

	// this time drawing is for shadows (except for top view)
	if (!tw->topCam->value()) {
		setupShadows();
		drawStuff(true);
		unsetupShadows();
	}

}

// note: this sets up both the Projection and the ModelView matrices
// HOWEVER: it doesn't clear the projection first (the caller handles
// that) - its important for picking
void TrainView::setProjection()
{
	// compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(w()) / static_cast<float>(h());

	if (tw->worldCam->value())
		arcball.setProjection(false);
	else if (tw->topCam->value()) {
		float wi,he;
		if (aspect >= 1) {
			wi = 110;
			he = wi/aspect;
		} else {
			he = 110;
			wi = he*aspect;
		}
		glMatrixMode(GL_PROJECTION);
		glOrtho(-wi,wi,-he,he,200,-200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-90,1,0,0);
	} else {
		// TODO: put code for train view projection here!
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, aspect, 0.1, 1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(this->world->xaxis, this->world->yaxis + 5, this->world->zaxis, this->world->viewx, this->world->viewy + 5, this->world->viewz, 0, 1, 0);

	}
}


void TrainView::loadTex(int i,  char* path)
{
		texs[i].image = TextureLoadBitmap(path,&texs[i].width,&texs[i].height);
		glGenTextures(1,&texs[i].texnum);
		glBindTexture(GL_TEXTURE_2D,texs[i].texnum);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//Assign the image to a texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texs[i].width, 
			texs[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
			texs[i].image);
}

// TODO: function that draws the track
void TrainView::drawTrack(bool doingShadows)
{
	//draw track with c0 coutinuity
	//linear version
	if (this->tw->splineBrowser->value() == 1){
		glPushMatrix();
		for (int i = 0; i < (this->world->points.size()); i++){

			float presentX = this->world->points[i].pos.x;
			float presentY = this->world->points[i].pos.y;
			float presentZ = this->world->points[i].pos.z;
			
			int p = i + 1;
			if (i == (this->world->points.size()) - 1) p = 0;
			float nextX = this->world->points[p].pos.x;
			float nextY = this->world->points[p].pos.y;
			float nextZ = this->world->points[p].pos.z;


			glBegin(GL_LINES);
			if(!doingShadows) glColor3f(1, 0.6, .8);
			glVertex3f(presentX, presentY, presentZ);
			glVertex3f(nextX, nextY, nextZ);
			glEnd();

			double length = sqrt((presentX - nextX)*(presentX - nextX) + (presentZ - nextZ)*(presentZ - nextZ));
			//the directional vector
			double xbias, ybias, zbias;
			float xdis = nextX - presentX;
			float ydis = nextY - presentY;
			float zdis = nextZ - presentZ;
			float theAngle;
			float highAngle;
		
			if (xdis >= 0 && zdis >= 0){
				theAngle = 180 - atan(zdis / xdis) * 360 / 6.28;
			}
			else if (xdis > 0 && zdis < 0){
				zdis = (-1) * zdis;
				theAngle = (-1)*(180 - atan(zdis / xdis) * 360 / 6.28);
			}
			else if (xdis < 0 && zdis < 0){
				xdis = (-1) * xdis;
				zdis = (-1) * zdis;
				theAngle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
			}
			else{
				xdis = (-1)*xdis;
				theAngle = atan(zdis / xdis) * 360 / 6.28;
			}
			highAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;
			float xo, yo, zo,orient_length;;

			//Now, draw the inside track
			//this is the wood track
			for (int times = 0; times / length < 0.98; times++){
				float ratio = times/length;
				float porient_x = this->world->points[i].orient.x;
				float porient_y = this->world->points[i].orient.y;
				float porient_z = this->world->points[i].orient.z;

				float norient_x = this->world->points[p].orient.x;
				float norient_y = this->world->points[p].orient.y;
				float norient_z = this->world->points[p].orient.z;
				
				xo = (1 - ratio)*porient_x + ratio*norient_x;
				yo = (1 - ratio)*porient_y + ratio*norient_y;
				zo = (1 - ratio)*porient_z + ratio*norient_z;
				orient_length = sqrt(xo*xo+yo*yo+zo*zo);
				times = times + 5;
				float norm_angle = acos(yo/orient_length) / PI * 180;
				float axis_x = zo, axis_y = 0, axis_z = -xo;
				//how much moved. tangent vector
				xbias = presentX + (nextX - presentX) * (times / length);
				ybias = presentY + (nextY - presentY) * (times / length);
				zbias = presentZ + (nextZ - presentZ) * (times / length);
				//calculate the board directional vector
				glPushMatrix();
				glTranslatef(xbias, ybias - 4, zbias);
				glRotatef(norm_angle, axis_x, axis_y, axis_z);
				//rotate in x-z	
				glRotatef(90 + theAngle, 0, 1, 0);
				//rotate in yz
				glRotatef(highAngle, 1, 0, 0);
				//rotate for orientation
				glTranslatef(-5, 0, -1);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,texs[0].texnum);
				glBegin(GL_QUADS);
				if (!doingShadows) glColor3f(.7, .7, .7);
				glTexCoord2f(0.0, 0.0); glVertex3f(0, 4, 0); 
				glTexCoord2f(0.0, 1.0); glVertex3f(10, 4, 0);
				glTexCoord2f(1.0, 1.0); glVertex3f(10, 4, 2);
				glTexCoord2f(1.0, 0.0); glVertex3f(0, 4, 2);
				glEnd();
				glDisable(GL_TEXTURE_2D);
				glPopMatrix();

			}
		}
		glPopMatrix();
	}

	float ts = this->tw->tension->value()/5.0 + 0.5;
	//draw track with c1 continuity
	//cardinal cubic
	/*
							  [-ts 2-ts ts-2  ts ][P(0)]
							  [2ts ts-3 3-2ts -ts][P(1)]
	p(t) = 1/2 * [t^3 t^2 t 1][-ts  0    ts    0 ][P(2)]
							  [ 0   1    0     0 ][P(3)]
	//P(0) here is the first point; P(1) 

	P(1) and P(2) are present and next control points
	*/
	if (this->tw->splineBrowser->value() == 2){
		glPushMatrix();
		if (!doingShadows) glColor3f(0.867, 0.427, 0.133);
		//traverse through all control points 
		for (int i = 0; i < (this->world->points.size()); i++){
			int p;
			//P[0]
			if (i == 0) p = this->world->points.size() - 1;
			else p = i - 1;
			float p0x = this->world->points[p].pos.x;
			float p0y = this->world->points[p].pos.y;
			float p0z = this->world->points[p].pos.z;
			//p[1]
			float p1x = this->world->points[i].pos.x;
			float p1y = this->world->points[i].pos.y;
			float p1z = this->world->points[i].pos.z;
			//get the orientation of points
			float porient_x = this->world->points[i].orient.x;
			float porient_y = this->world->points[i].orient.y;
			float porient_z = this->world->points[i].orient.z;
			//P[2]
			p = (i + 1) % (this->world->points.size());
			float p2x = this->world->points[p].pos.x;
			float p2y = this->world->points[p].pos.y;
			float p2z = this->world->points[p].pos.z;
			float norient_x = this->world->points[p].orient.x;
			float norient_y = this->world->points[p].orient.y;
			float norient_z = this->world->points[p].orient.z;
			//P[3]
			p = (i + 2) % (this->world->points.size());
			float p3x = this->world->points[p].pos.x;
			float p3y = this->world->points[p].pos.y;
			float p3z = this->world->points[p].pos.z;

			//calculate distance between P1 and P2
			float length = sqrt((p1x - p2x)*(p1x - p2x) + (p1y - p2y)*(p1y - p2y) + (p1z - p2z)*(p1z - p2z));

			//calculate the matrix
			float block = 0.0; // use this parameter to calculate where I need to put a wood of track in the this->world
			// because when you prolong the track the ratio of distance may change, so we need to figure out a way to fix it
			//record where the last wood is
			float lastBlockX = p1x;
			float lastBlockY = p1y;
			float lastBlockZ = p1z;
			float xo;
			float yo;
			float zo;
			float orient_length;
			//now, interpolate based on 4 control pointss
			for (float t = 0; t < 1; t += 0.001){
				//interpolated points
				float px, py, pz;
				px = (((-ts)*p0x + (2-ts) * p1x + (ts-2)*p2x + ts*p3x)*t*t*t + (2*ts * p0x + (ts-3) * p1x + (3-2*ts) * p2x - ts*p3x)*t*t + ((-ts)*p0x + ts*p2x)*t +  p1x);
				py = (((-ts)*p0y + (2-ts) * p1y + (ts-2)*p2y + ts*p3y)*t*t*t + (2*ts * p0y + (ts-3) * p1y + (3-2*ts) * p2y - ts*p3y)*t*t + ((-ts)*p0y + ts*p2y)*t +  p1y);
				pz = (((-ts)*p0z + (2-ts) * p1z + (ts-2)*p2z + ts*p3z)*t*t*t + (2*ts * p0z + (ts-3) * p1z + (3-2*ts) * p2z - ts*p3z)*t*t + ((-ts)*p0z + ts*p2z)*t +  p1z);

				glBegin(GL_POINTS);
				if(!doingShadows) glColor3f(1, 0.6, .8);
				glVertex3f(px, py, pz);
				glEnd();
				block += 0.001;
				if (block * length >= 6){
					xo = (1 - t)*porient_x + t*norient_x;
					yo = (1 - t)*porient_y + t*norient_y;
					zo = (1 - t)*porient_z + t*norient_z;
					orient_length = sqrt(xo*xo+yo*yo+zo*zo);
					//the displacement vector
					float xdis = px - lastBlockX;
					float ydis = py - lastBlockY;
					float zdis = pz - lastBlockZ;
					//to calculate the rotate angle
					float theAngle;  // this parameter is the rotate angle in space X-Z
					float highAngle; // this parameter is the rotate angle in space X-Y
					if (xdis >= 0 && zdis >= 0){
						theAngle = 180 - atan(zdis / xdis) * 360 / 6.28;
					}
					else if (xdis > 0 && zdis < 0){
						theAngle = (-1)*(180 - atan((-1)*zdis / xdis) * 360 / 6.28);
					}
					else if (xdis < 0 && zdis < 0){
						theAngle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
					}
					else{
						theAngle = atan((-1)*zdis / xdis) * 360 / 6.28;
					}
					highAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;
					//calculate rotate matrix
					float norm_angle = acos(yo/orient_length) / PI * 180;
					float axis_x = zo, axis_y = 0, axis_z = -xo;
					glPushMatrix();
						glTranslatef(px, py, pz);
						glRotatef(norm_angle, axis_x, axis_y, axis_z);
						//rotate the X-Z angle
						glRotatef(90 + theAngle, 0, 1, 0);
						//rotate the Y-Z angle
						glRotatef(highAngle, 1, 0, 0);
						//rotate the X-Y angle
						//glRotatef(-zangle,0,0,1);
						glTranslatef(-5, 0, -1);
						//now, draw the track
						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, texs[0].texnum);
						glBegin(GL_QUADS);
						if (!doingShadows) glColor3f(0.7, 0.7, 0.7);
						glTexCoord2f(0.0, 0.0);	glVertex3f(0, 0, 0);
						glTexCoord2f(0.0, 1.0);	glVertex3f(10, 0, 0);
						glTexCoord2f(1.0, 1.0);	glVertex3f(10, 0, 2);
						glTexCoord2f(1.0, 0.0);	glVertex3f(0, 0, 2);
						glEnd();
						glDisable(GL_TEXTURE_2D);
					glPopMatrix();
					block = 0;
					lastBlockX = px;
					lastBlockY = py;
					lastBlockZ = pz;
				}
			}
		}
		glPopMatrix();
	}
	
}
float step = 0.4;
//TODO: function that draw the train
void TrainView::drawTrain(bool doingShadows)
{
	
	if(this->tw->carBrowser->value()==1)
	{
	glPushMatrix();
	glScalef(0.4,0.55,0.8);
	glTranslatef(0, 0, -5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texs[2].texnum);
	//for the body part
	glBegin(GL_QUADS);
	// front
	//(x-axis, z-axis, y-axis)
	if (!doingShadows) glColor3f(0.453f, 0.219f, 0.035f);
	glTexCoord2f(0.0,0.0);	glVertex3f(0, 4, 0);
	glTexCoord2f(0.0,1.0);	glVertex3f(0, 14, 0);
	glTexCoord2f(1.0,1.0);	glVertex3f(0, 14, 10);
	glTexCoord2f(1.0,0.0);	glVertex3f(0, 4, 10);

	// back
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glTexCoord2f(0.0,0.0);	glVertex3f(30, 4, 0);
	glTexCoord2f(0.0,1.0);	glVertex3f(30, 14, 0);
	glTexCoord2f(1.0,1.0);	glVertex3f(30, 14, 10);
	glTexCoord2f(1.0,0.0);	glVertex3f(30, 4, 10);

	// top
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glTexCoord2f(0.0,0.0);	glVertex3f(0, 14, 0);
	glTexCoord2f(0.0,1.0);	glVertex3f(30, 14, 0);
	glTexCoord2f(1.0,1.0);	glVertex3f(30, 14, 10);
	glTexCoord2f(1.0,0.0);	glVertex3f(0, 14, 10);

	// bottom
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glTexCoord2f(0.0,0.0);	glVertex3f(0, 4, 0);
	glTexCoord2f(0.0,1.0);	glVertex3f(30, 4, 0);
	glTexCoord2f(1.0,1.0);	glVertex3f(30, 4, 10);
	glTexCoord2f(1.0,0.0);	glVertex3f(0, 4, 10);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texs[1].texnum);
	glBegin(GL_QUADS);
	// left
	if (!doingShadows) glColor3f(0.365f, 0.580f, 0.102f);
	glTexCoord2f(0.0,0.0);	glVertex3f(0, 4, 0);
	glTexCoord2f(0.0,1.0);	glVertex3f(0, 14, 0);
	glTexCoord2f(1.0,1.0);	glVertex3f(30, 14, 0);
	glTexCoord2f(1.0,0.0);	glVertex3f(30, 4, 0);

	// right
	if (!doingShadows) glColor3f(0.565f, 0.580f, 0.102f);
	glTexCoord2f(0.0,0.0);	glVertex3f(0, 4, 10);
	glTexCoord2f(0.0,1.0);	glVertex3f(0, 14, 10);
	glTexCoord2f(1.0,1.0);	glVertex3f(30, 14, 10);
	glTexCoord2f(1.0,0.0);	glVertex3f(30, 4, 10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//for the higher part
	glBegin(GL_QUADS);
	// front
	//(x-axis, z-axis, y-axis)
	if (!doingShadows) glColor3f(0.965f, 0.580f, 0.102f);
	glVertex3f(20, 14, 0);
	glVertex3f(20, 22, 0);
	glVertex3f(20, 22, 10);
	glVertex3f(20, 14, 10);

	
	// back
	if (!doingShadows) glColor3f(0.968f, 0.219f, 0.035f);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 22, 0);
	glVertex3f(30, 22, 10);
	glVertex3f(30, 14, 10);

	// top
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(20, 22, 0);
	glVertex3f(30, 22, 0);
	glVertex3f(30, 22, 10);
	glVertex3f(20, 22, 10);

	// bottom
	if (!doingShadows) glColor3f(0.067f, 0.761f, 0.937f);
	glVertex3f(20, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 14, 10);
	glVertex3f(20, 14, 10);

	// left
	if (!doingShadows) glColor3f(0.365f, 0.580f, 0.102f);
	glVertex3f(20, 14, 0);
	glVertex3f(30, 14, 0);
	glVertex3f(30, 22, 0);
	glVertex3f(20, 22, 0);

	// right
	if (!doingShadows) glColor3f(0.365f, 0.580f, 0.102f);
	glVertex3f(20, 14, 10);
	glVertex3f(30, 14, 10);
	glVertex3f(30, 22, 10);
	glVertex3f(20, 22, 10);
	glEnd();


	//for the wheels part
	//left and front wheel
	glPushMatrix();
	glTranslated(7.5, 4, 10);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += step)
	{
		for (float j = 0; j < 8 * 3.14; j += step)
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
	for (float k = 0; k < 2 * 3.14; k += step)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + step), sin(k + step), 0);
		glEnd();
	}
	glPopMatrix();

	//left and back wheel
	glPushMatrix();
	glTranslated(22.5, 4, 10);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += 0.15f)
	{
		for (float j = 0; j < 8 * 3.14; j += step)
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
	glTranslated(22.5, 4, 12);
	glScaled(3, 3, 1);
	for (float k = 0; k < 2 * 3.14; k += step)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + step), sin(k + step), 0);
		glEnd();
	}
	glPopMatrix();

	//right and front wheel
	glPushMatrix();
	glTranslated(7.5, 4, -2);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += step)
	{
		for (float j = 0; j < 8 * 3.14; j += step)
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
	for (float k = 0; k < 2 * 3.14; k += step)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0, 0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + step), sin(k + step), 0);
		glEnd();
	}
	glPopMatrix();


	//right and back wheel
	glPushMatrix();
	glTranslated(22.5, 4, -2);
	glScaled(3, 3, 1);
	for (float i = 0; i < 1; i += step)
	{
		for (float j = 0; j < 8 * 3.14; j += step)
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
	for (float k = 0; k < 2 * 3.14; k += step)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(cos(k), sin(k), 0);
		glVertex3f(cos(k + step), sin(k + step), 0);
		glEnd();
	}
	glPopMatrix();


	//for the funnel part
	glPushMatrix();
	glTranslated(7.5, 14, 5);
	glScaled(3, 1, 3);
	if (!doingShadows) glColor3f(0.2, 0.3, 0.4);
	for (float j = 0; j < 8 * 3.14; j += step)
	{
		glBegin(GL_LINES);
		glVertex3f(cos(j), 0, sin(j));
		glVertex3f(cos(j), 6, sin(j));
		glEnd();
	}
	glPopMatrix();
	glPushMatrix();
	glTranslated(7.5, 19, 5);
	glScaled(3, 1, 3);
	for (float k = 0; k < 2 * 3.14; k += step)
	{
		glBegin(GL_TRIANGLES);
		if (!doingShadows) glColor3f(0.2, 0.2, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(k), 0, sin(k));
		glVertex3f(cos(k + step), 0, sin(k + step));
		glEnd();
	}
	glPopMatrix();
	glPopMatrix();
	}

	if(this->tw->carBrowser->value()==2)
	{
		glPushMatrix();
		int x = 10;
		int z = 0;
		glScalef(0.70,1.5,1);
		glTranslated(x, 0, z);
		glRotated(180, 0, 1, 0);
		//interior
		if (!doingShadows)
			glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_QUADS);//base
		glVertex3f(-10, 0.1, -4.9);
		glVertex3f(-10, 0.1, 4.9);
		glVertex3f(10, 0.1, 4.9);
		glVertex3f(10, 0.1, -4.9);
		glEnd();
	
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texs[1].texnum);

		glBegin(GL_QUADS);//wall
		glTexCoord2f(0.0,0.0);glVertex3f(-10, 0, -4.9);
		glTexCoord2f(0.0,1.0);glVertex3f(-10, 5, -4.9);
		glTexCoord2f(1.0,1.0);glVertex3f(2, 5, -4.9);
		glTexCoord2f(1.0,0.0);glVertex3f(10, 0, -4.9);
		glEnd();
		glBegin(GL_QUADS);//wall
		glTexCoord2f(0.0,0.0);glVertex3f(-10, 0, 4.9);
		glTexCoord2f(0.0,1.0);glVertex3f(-10, 5, 4.9);
		glTexCoord2f(1.0,1.0);glVertex3f(2, 5, 4.9);
		glTexCoord2f(1.0,0.0);glVertex3f(10, 0, 4.9);
		glEnd();

		//exterior
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texs[3].texnum);
		if (!doingShadows)
			glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_QUADS);//wall
		glTexCoord2f(0.0,0.0);glVertex3f(-10, 0, -5);
		glTexCoord2f(0.0,1.0);glVertex3f(-10, 5, -5);
		glTexCoord2f(1.0,1.0);glVertex3f(2, 5, -5);
		glTexCoord2f(1.0,0.0);glVertex3f(10, 0, -5);
		glEnd();
		glBegin(GL_QUADS);//wall-seat-edge
		glVertex3f(-4, 5, -5);
		glVertex3f(-4, 7, -5);
		glVertex3f(-3, 7, -5);
		glVertex3f(-19.0 / 7.0, 5, -5);
		glEnd();
		
		glBegin(GL_QUADS);//wall
		glTexCoord2f(0.0,0.0);glVertex3f(-10, 0, 5);
		glTexCoord2f(0.0,1.0);glVertex3f(-10, 5, 5);
		glTexCoord2f(1.0,1.0);glVertex3f(2, 5, 5);
		glTexCoord2f(1.0,0.0);glVertex3f(10, 0, 5);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);//wall-seat-edge
		glVertex3f(-4, 5, 5);
		glVertex3f(-4, 7, 5);
		glVertex3f(-3, 7, 5);
		glVertex3f(-19.0 / 7.0, 5, 5);
		glEnd();

		glBegin(GL_QUADS);//back
		glVertex3f(-10, 0, 5);
		glVertex3f(-10, 0, -5);
		glVertex3f(-10, 7, -5);
		glVertex3f(-10, 7, 5);
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texs[1].texnum);

		glBegin(GL_QUADS);//front
		glTexCoord2f(0.0,0.0);glVertex3f(10, 0, 5);
		glTexCoord2f(0.0,1.0);glVertex3f(2, 5, 5);
		glTexCoord2f(1.0,1.0);glVertex3f(2, 5, -5);
		glTexCoord2f(1.0,0.0);glVertex3f(10, 0, -5);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);//wall--back-seat-edge
		glVertex3f(-10, 5, -5);
		glVertex3f(-10, 7, -5);
		glVertex3f(-9, 7, -5);
		glVertex3f(-61.0 / 7.0, 5, -5);
		glEnd();

		glBegin(GL_QUADS);//wall-back-seat-edge
		glVertex3f(-10, 5, 5);
		glVertex3f(-10, 7, 5);
		glVertex3f(-9, 7, 5);
		glVertex3f(-61.0 / 7.0, 5, 5);
		glEnd();

		glBegin(GL_QUADS);//wall-back-seat-edge
		glVertex3f(-10, 5, -5);
		glVertex3f(-10, 7, -5);
		glVertex3f(-9, 7, -5);
		glVertex3f(-61.0 / 7.0, 5, -5);
		glEnd();

		glBegin(GL_QUADS);//wall-back-seat-edge
		glVertex3f(-10, 5, 5);
		glVertex3f(-10, 7, 5);
		glVertex3f(-9, 7, 5);
		glVertex3f(-61.0 / 7.0, 5, 5);
		glEnd();

		//seats
		if (!doingShadows)
			glColor3f(0.2, 0.2, 0.2);
		glBegin(GL_QUADS);//seat-back
		glVertex3f(-4, 0, 5);
		glVertex3f(-4, 7, 5);
		glVertex3f(-4, 7, -5);
		glVertex3f(-4, 0, -5);
		glEnd();
		glBegin(GL_QUADS);//seat-top
		glVertex3f(-4, 7, 5);
		glVertex3f(-4, 7, -5);
		glVertex3f(-3, 7, -5);
		glVertex3f(-3, 7, 5);
		glEnd();
		glBegin(GL_QUADS);//seat-seat
		glVertex3f(-3, 7, 5);
		glVertex3f(-3, 7, -5);
		glVertex3f(-2, 0, -5);
		glVertex3f(-2, 0, 5);
		glEnd();

		glBegin(GL_QUADS);//seat-top
		glVertex3f(-10, 7, 5);
		glVertex3f(-10, 7, -5);
		glVertex3f(-9, 7, -5);
		glVertex3f(-9, 7, 5);
		glEnd();
		glBegin(GL_QUADS);//seat-seat
		glVertex3f(-9, 7, 5);
		glVertex3f(-9, 7, -5);
		glVertex3f(-8, 0, -5);
		glVertex3f(-8, 0, 5);
		glEnd();

		glPopMatrix();
	}
}

void TrainView::drawParticle(bool doingShadows)
{
	static int col=6;
	glPushMatrix();
	for(int loop=0;loop<MAX_NUM;loop++)
	{
		if (particle[loop].active)					// If The Particle Is Active
		{
			float x=particle[loop].x;				// Grab Our Particle X Position
			float y=particle[loop].y;				// Grab Our Particle Y Position
			float z=particle[loop].z;				// Particle Z Pos + Zoom
			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			if(!doingShadows)
				glColor4f(particle[loop].r,particle[loop].g,particle[loop].b,particle[loop].life);
			glTranslatef(particle[loop].x,particle[loop].y,particle[loop].z);
			gluSphere(cylinder,0.2f,4,4);
			glTranslatef(-particle[loop].x,-particle[loop].y,-particle[loop].z);
		
			particle[loop].x+=particle[loop].xi/(10*100);	// Move On The X Axis By X Speed
			particle[loop].y+=particle[loop].yi/(10*100);	// Move On The Y Axis By Y Speed
			particle[loop].z+=particle[loop].zi/(10*100);	// Move On The Z Axis By Z Speed
			particle[loop].xi+=particle[loop].xg;			// Take Pull On X Axis Into Account
			particle[loop].yi+=particle[loop].yg;			// Take Pull On Y Axis Into Account. Here YG is negative
			particle[loop].zi+=particle[loop].zg;			// Take Pull On Z Axis Into Account
			particle[loop].life-=particle[loop].fade;		// Reduce Particles Life By 'Fade'
			
			if (particle[loop].life<0.0f || particle[loop].y < 0)					// If Particle Is Burned Out
			{
				particle[loop].life=1.0f;				// Give It New Life
				particle[loop].fade=float(rand()%100)/1000.0f+0.003f;	// Random Fade Value
				particle[loop].x=0.0f;					// Center On X Axis
				particle[loop].y=5.f;					// Center On Y Axis
				particle[loop].z=0.0f;					// Center On Z Axis
				particle[loop].xi= -100.f + float((rand()%200));	// X Axis Speed And Direction
				particle[loop].yi= 50.f + float((rand()%550));	// Y Axis Speed And Direction
				particle[loop].zi= -100.f + float((rand()%200));		// Z Axis Speed And Direction
				particle[loop].r=colors[(loop+1)/(MAX_NUM/12)][0];	// Select Red Rainbow Color
				particle[loop].g=colors[(loop+1)/(MAX_NUM/12)][1];	// Select Green Rainbow Color
				particle[loop].b=colors[(loop+1)/(MAX_NUM/12)][2];	// Select Blue Rainbow Color
			}
		}	
	}	
	glPopMatrix();


}

// this draws all of the stuff in the world
// NOTE: if you're drawing shadows, DO NOT set colors 
// (otherwise, you get colored shadows)
// this gets called twice per draw - once for the objects, once for the shadows
// TODO: if you have other objects in the world, make sure to draw them
void TrainView::drawStuff(bool doingShadows)
{
	// draw the control points
	// don't draw the control points if you're driving 
	if (!tw->trainCam->value()) {
		for(size_t i=0; i<world->points.size(); ++i) {
			if (!doingShadows) {
				if ( ((int) i) != selectedCube)
					glColor3ub(240,60,60);
				else
					glColor3ub(240,240,30);
			}
			world->points[i].draw();
		}
	}

	MyObjects drawit;
	// draw the fountain
	drawParticle(doingShadows);
	// draw the track
	// TODO: call your own track drawing code
	drawTrack(doingShadows);
	// draw the train
	// TODO: call your own train drawing code
	// don't draw the train if you're looking out the front window
	if (!tw->trainCam->value())
	{
		glPushMatrix();
			glTranslatef(this->world->xaxis,this->world->yaxis,this->world->zaxis);
			glRotatef(this->world->or_angle, this->world->txo, this->world->tyo, this->world->tzo);
			printf("%f,%f,%f,%f\n",this->world->or_angle, this->world->txo, this->world->tyo, this->world->tzo);
			glRotatef(this->world->angle, 0, 1, 0);
			glRotatef(-this->world->heightAngle, 0, 0, 1);
			drawTrain(doingShadows);
		glPopMatrix();
		for(int i=0; i<tw->world.trainnum;i++)
		{
			//determine the position of the rest
			//linear version
			glPushMatrix();
				newcar cur = this->newcars[i];
				glTranslatef(cur.xaxis,cur.yaxis,cur.zaxis);
				glRotatef(cur.o_angle,cur.o_xo,cur.o_yo,cur.o_zo);
				glRotatef(cur.angle, 0, 1, 0);
				glRotatef(-cur.heightangle, 0, 0, 1);
				drawTrain(doingShadows);
			glPopMatrix();
		}
	}
	drawit.drawTrees(this,doingShadows);
	drawit.drawBallon(this,doingShadows);
	drawit.drawWindHouse(this,doingShadows);
	glFlush();
}

// this tries to see which control point is under the mouse
// (for when the mouse is clicked)
// it uses OpenGL picking - which is always a trick
// TODO: if you want to pick things other than control points, or you
// changed how control points are drawn, you might need to change this
void TrainView::doPick()
{
	make_current();		// since we'll need to do some GL stuff

	int mx = Fl::event_x(); // where is the mouse?
	int my = Fl::event_y();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	// set up the pick matrix on the stack - remember, FlTk is
	// upside down!
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluPickMatrix((double)mx, (double)(viewport[3]-my), 5, 5, viewport);

	// now set up the projection
	setProjection();

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100,buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	// draw the cubes, loading the names as we go
	for(size_t i=0; i<world->points.size(); ++i) {
		glLoadName((GLuint) (i+1));
		world->points[i].draw();
	}

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selectedCube = buf[3]-1;
	} else // nothing hit, nothing selected
		selectedCube = -1;

	printf("Selected Cube %d\n",selectedCube);
}



// CVS Header - if you don't know what this is, don't worry about it
// This code tells us where the original came from in CVS
// Its a good idea to leave it as-is so we know what version of
// things you started with
// $Header: /p/course/-gleicher/private/CVS/TrainFiles/TrainView.cpp,v 1.9 2008/10/21 14:46:45 gleicher Exp $
