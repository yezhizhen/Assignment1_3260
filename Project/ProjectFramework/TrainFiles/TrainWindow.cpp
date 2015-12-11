//  Train Project -
// Train Window class implementation
// - note: this is code that a student might want to modify for their project
//   see the header file for more details
// - look for the TODO: in this file
// - also, look at the "TrainView" - its the actual OpenGL window that
//   we draw into
//
// Written by Mike Gleicher, October 2008
//

//////////////////////////////////////////////////////////////////////////
//Please fill your name and student ID
//Name: YE ZHIZHEN
//StuID: 1155046993
//////////////////////////////////////////////////////////////////////////


#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"
#include <FL/fl.h>
#include <FL/Fl_Box.h>
#define PI_HERE 3.14159265358979f
// for using the real time clock
#include <time.h>

#ifndef TRAIN_DIS
#define TRAIN_DIS 19  
#endif // !TRAIN_DIS


////////////////////////////////////////////////////
TrainWindow::TrainWindow(const int x, const int y) : Fl_Double_Window(x,y,800,600,"Train and Roller Coaster")
{
	// make all of the widgets
	begin();	// add to this widget
	{
		int pty=5;			// where the last widgets were drawn. From top down

		trainView = new TrainView(5,5,590,590);
		trainView->tw = this;
		trainView->world = &world;
		this->resizable(trainView);

		// to make resizing work better, put all the widgets in a group
		widgets = new Fl_Group(600,5,190,590);
		widgets->begin();

		runButton = new Fl_Button(605,pty,60,20,"Run");
		togglify(runButton);

		Fl_Button* fb = new Fl_Button(700,pty,25,20,"@>>");
		fb->callback((Fl_Callback*)forwCB,this);
		Fl_Button* rb = new Fl_Button(670,pty,25,20,"@<<");
		rb->callback((Fl_Callback*)backCB,this);
		
		arcLength = new Fl_Button(730,pty,65,20,"ArcLength");
		togglify(arcLength,1);
  
		pty+=25;
		speed = new Fl_Value_Slider(655,pty,140,20,"speed");
		speed->range(0,10);
		speed->value(2);
		speed->align(FL_ALIGN_LEFT);
		speed->type(FL_HORIZONTAL);

		pty += 30;

		// camera buttons - in a radio button group
		Fl_Group* camGroup = new Fl_Group(600,pty,195,20);
		camGroup->begin();
		worldCam = new Fl_Button(605, pty, 60, 20, "World");
        worldCam->type(FL_RADIO_BUTTON);		// radio button
        worldCam->value(1);			// turned on
        worldCam->selection_color((Fl_Color)3); // yellow when curpsed
		worldCam->callback((Fl_Callback*)damageCB,this);
		trainCam = new Fl_Button(670, pty, 60, 20, "Train");
        trainCam->type(FL_RADIO_BUTTON);
        trainCam->value(0);
        trainCam->selection_color((Fl_Color)3);
		trainCam->callback((Fl_Callback*)damageCB,this);
		topCam = new Fl_Button(735, pty, 60, 20, "Top");
        topCam->type(FL_RADIO_BUTTON);
        topCam->value(0);
        topCam->selection_color((Fl_Color)3);
		topCam->callback((Fl_Callback*)damageCB,this);
		camGroup->end();

		pty += 30;

		// browser to select spline types
		// TODO: make sure these choices are the same as what the code supports
		splineBrowser = new Fl_Browser(605,pty,120,75,"Spline Type");
		splineBrowser->type(2);		// select
		splineBrowser->add("Linear");
		splineBrowser->add("Cardinal Cubic");
		splineBrowser->select(2);
		splineBrowser->callback((Fl_Callback*)changeSmoothness,this);
		pty += 110;

		// add and delete points
		Fl_Button* ap = new Fl_Button(605,pty,80,20,"Add Point");
		ap->callback((Fl_Callback*)addPointCB,this);
		Fl_Button* dp = new Fl_Button(690,pty,80,20,"Delete Point");
		dp->callback((Fl_Callback*)deletePointCB,this);

		pty += 25;
		// reset the points
		resetButton = new Fl_Button(735,pty,60,20,"Reset");
		resetButton->callback((Fl_Callback*)resetCB,this);
		Fl_Button* loadb = new Fl_Button(605,pty,60,20,"Load");
		loadb->callback((Fl_Callback*) loadCB, this);
		Fl_Button* saveb = new Fl_Button(670,pty,60,20,"Save");
		saveb->callback((Fl_Callback*) saveCB, this);

		pty += 25;
		// roll the points
		Fl_Button* rx = new Fl_Button(605,pty,30,20,"R+X");
		rx->callback((Fl_Callback*)rpxCB,this);
		Fl_Button* rxp = new Fl_Button(635,pty,30,20,"R-X");
		rxp->callback((Fl_Callback*)rmxCB,this);
		Fl_Button* rz = new Fl_Button(670,pty,30,20,"R+Z");
		rz->callback((Fl_Callback*)rpzCB,this);
		Fl_Button* rzp = new Fl_Button(700,pty,30,20,"R-Z");
		rzp->callback((Fl_Callback*)rmzCB,this);

		pty+=40;

		// TODO: add widgets for all of your fancier features here
		tension = new Fl_Value_Slider(670,pty,120,20,"tension s");
		tension->range(0,6);
		tension->value(2);
		tension->align(FL_ALIGN_LEFT);
		tension->type(FL_HORIZONTAL);
		tension->callback((Fl_Callback*)damageCB,this);
		
		pty += 30;
		physics =  new Fl_Value_Slider(670,pty,120,20,"Physics");
		physics->range(2,15);
		physics->value(4.5);
		physics->align(FL_ALIGN_LEFT);
		physics->type(FL_HORIZONTAL);
		physics->callback((Fl_Callback*)damageCB,this);


		pty+=30;
		Physics = new Fl_Button(605, pty, 60, 20, "Physics");
		togglify(Physics);

		pty+=30;
		Fl_Button* at = new Fl_Button(605,pty,80,20,"Add Train");
		at->callback((Fl_Callback*)addTrainCB,this);

		Fl_Button* dt = new Fl_Button(690,pty,80,20,"Delete Train");
		dt->callback((Fl_Callback*)deleteTrainCB,this);
		
		pty+=20;
		carBrowser = new Fl_Browser(605,pty,120,55,"Train Type");
		carBrowser->type(2);		// select
		carBrowser->add("Classical");
		carBrowser->add("Roller Coaster");
		carBrowser->select(2);
		carBrowser->callback((Fl_Callback*)damageCB,this);
		
		pty+=80;
		Fog = new Fl_Button(605,pty,65,20,"Fog");
		togglify(Fog,0);

		pty+= 30;
		FOG_SLIDER =  new Fl_Value_Slider(655,pty,120,20,"density");
		FOG_SLIDER->range(0.5,6);
		FOG_SLIDER->value(1.5);
		FOG_SLIDER->align(FL_ALIGN_LEFT);
		FOG_SLIDER->type(FL_HORIZONTAL);
		FOG_SLIDER->callback((Fl_Callback*)damageCB,this);

		// we need to make a little phantom widget to have things resize correctly
		Fl_Box* resizebox = new Fl_Box(600,595,200,5);
		widgets->resizable(resizebox);

		widgets->end();
	}
	end();	// done adding to this widget
	// set up callback on idle
	Fl::add_idle((void (*)(void*))runButtonCB,this);
}

// handy utility to make a button into a toggle
void TrainWindow::togglify(Fl_Button* b, int val)
{
    b->type(FL_TOGGLE_BUTTON);		// toggle
    b->value(val);		// turned off
    b->selection_color((Fl_Color)3); // yellow when curpsed	
	b->callback((Fl_Callback*)damageCB,this);
}

void TrainWindow::damageMe()
{
	//the second is the number of control points
	//
	if (trainView->selectedCube >= ((int)world.points.size()))
		trainView->selectedCube = 0;
	trainView->damage(1);
}

/////////////////////////////////////////////////////
// this will get called (approximately) 30 times per second
// if the run button is curpsed
void TrainWindow::advanceTrain(float dir)
{

	// TODO: make this work for your train. Make the train move forword
	
	float speed = this->speed->value();
	//if is in train view
	if(this->trainCam->value())	speed /=3;
	float ts = this->tension->value()/5.0 + 0.5;

	//define the strand length. So the train can move the fixed 
	float strandlength = 68.134;
	bool isarc = false;
	//if arc is activated
	if(this->arcLength->value())	isarc=true;
	//if we use arc length, then keep this constant for linear version
	float rate = 0.6;
	//linear case. C0 continuity
	if(this->world.smoothness == 0)
	{
		//on the current control point
		int curn = int(this->world.trainU);
		int nexn;
		//set 
		ControlPoint curp = this->world.points[curn];
		ControlPoint next;
		if (curn == this->world.points.size() - 1) 
		{
			next = this->world.points[0];
			nexn = 0;
		}
		else 
		{
			nexn = curn +1;
			next = this->world.points[curn + 1];
		}
		double length = sqrt((curp.pos.x - next.pos.x)*(curp.pos.x - next.pos.x) + (curp.pos.z - next.pos.z)*(curp.pos.z - next.pos.z));
		float porient_x = curp.orient.x;
		float porient_y = curp.orient.y;
		float porient_z = curp.orient.z;

		float norient_x = next.orient.x;
		float norient_y = next.orient.y;
		float norient_z = next.orient.z;
		//not arc version
		if(!isarc)
		{
			//to make sure moves same part of curve
			rate = length/strandlength;
		}

		//set train view properly
		this->world.viewx = next.pos.x;
		this->world.viewy = next.pos.y;
		this->world.viewz = next.pos.z;

		//displacement vector
		float xdis = next.pos.x - curp.pos.x;
		float ydis = next.pos.y - curp.pos.y;
		float zdis = next.pos.z - curp.pos.z;
		if (xdis >= 0 && zdis >= 0){
			this->world.angle = 180 - atan(zdis / xdis) * 360 / 6.28;
		}
		else if (xdis > 0 && zdis < 0){
			zdis = (-1) * zdis;
			this->world.angle = (-1)*(180 - atan(zdis / xdis) * 360 / 6.28);
		}
		else if (xdis < 0 && zdis < 0){
			xdis = (-1) * xdis;
			zdis = (-1) * zdis;
			this->world.angle = (-1)*(atan(zdis / xdis) * 360 / 6.28);
		}
		else{
			xdis = (-1)*xdis;
			this->world.angle = atan(zdis / xdis) * 360 / 6.28;
		}

		this->world.heightAngle = atan(ydis / (sqrt(xdis*xdis + zdis*zdis))) * 360 / 6.28;

		//from current plus the length times the portion
		this->world.xaxis = curp.pos.x + (next.pos.x - curp.pos.x) * (this->world.distance / length);
		this->world.yaxis = curp.pos.y + (next.pos.y - curp.pos.y) * (this->world.distance / length);
		this->world.zaxis = curp.pos.z + (next.pos.z - curp.pos.z) * (this->world.distance / length);
		float xo = (1 - this->world.distance / length)*porient_x + this->world.distance / length*norient_x;
		float yo = (1 - this->world.distance / length)*porient_y + this->world.distance / length*norient_y;
		float zo = (1 - this->world.distance / length)*porient_z + this->world.distance / length*norient_z;
		float orient_length = sqrt(xo*xo+yo*yo+zo*zo);
		float norm_angle = acos(yo/orient_length) / 3.1415926 * 180;
		//float axis_x = zo, axis_y = 0, axis_z = -xo;
		this->world.txo =  zo;
		this->world.tyo = 0;
		this->world.tzo = -xo;
		this->world.or_angle = norm_angle;
		
		float phys;
		if (this->Physics->value()){
			//calculate the present Y-axis
			phys = this->physics->value() - 0.1 *(this->world.yaxis-5.f);
		}
		//no physics
		else{
			phys = this->physics->value();
		}
		//control the speed of the train
		float dis = 3 * speed*rate*phys/3.5;
		//how much you have moved?
		this->world.distance += dis;
		int preu = curn;
		int pren = nexn;
		float prelength = length;
		float preangle = this->world.angle;
		float preheightangle = this->world.heightAngle;
		//calculate the predistance
		float predistance = this->world.distance;
		//draw the new car
		for(int i=0;i<this->world.trainnum;i++)
		{
			ControlPoint cp = this->world.points[preu];
			ControlPoint np = this->world.points[pren];
			TrainView::newcar *current = &this->trainView->newcars[i];
			//float porient_x,porient_y,porient_z,norient_x,norient_y,norient_z;
			porient_x = cp.orient.x;
			porient_y = cp.orient.y;
			porient_z = cp.orient.z;
			
			norient_x = np.orient.x;
			norient_y = np.orient.y;
			norient_z = np.orient.z;
			//calculate the new delta for new train.
			//still on same part
			if(predistance>TRAIN_DIS)
			{

				//curp.pos.x + (next.pos.x - curp.pos.x) * (this->world.distance / length);
				current->u = preu;
				current->delta = (predistance-TRAIN_DIS)/prelength;
				current->xaxis = cp.pos.x + (np.pos.x-cp.pos.x)* current->delta;	
				current->yaxis = cp.pos.y + (np.pos.y-cp.pos.y)* current->delta;
				current->zaxis = cp.pos.z + (np.pos.z-cp.pos.z)* current->delta;
				xo = (1 - current->delta)*porient_x + current->delta*norient_x;
				yo = (1 - current->delta)*porient_y + current->delta*norient_y;
				zo = (1 - current->delta)*porient_z + current->delta*norient_z;
				orient_length = sqrt(xo*xo+yo*yo+zo*zo);
				norm_angle = acos(yo/orient_length) / PI_HERE * 180;
				current->angle = preangle;
				current->heightangle = preheightangle;
				current->o_xo = zo;	current->o_yo = 0;	current->o_zo = -xo;
				current->o_angle = norm_angle;
				predistance -= TRAIN_DIS;
			}
			//first carriage cannot be added
			else if( predistance<=TRAIN_DIS)
			{

				//set back to previous
				pren = preu;
				if(preu>0)	preu--;
				//0 case
				else    preu = this->world.points.size()-1;
				//now 
				cp = this->world.points[preu];
				np = this->world.points[pren];
				porient_x = cp.orient.x;
				porient_y = cp.orient.y;
				porient_z = cp.orient.z;
			
				norient_x = np.orient.x;
				norient_y = np.orient.y;
				norient_z = np.orient.z;
				//displacement vector
				float xdis_ = np.pos.x - cp.pos.x;
				float ydis_ = np.pos.y - cp.pos.y;
				float zdis_ = np.pos.z - cp.pos.z;
				if (xdis_ >= 0 && zdis_ >= 0){
					preangle = 180 - atan(zdis_ / xdis_) * 360 / 6.28;
				}
				else if (xdis_ > 0 && zdis_ < 0){
					zdis_ = (-1) * zdis_;
					preangle = (-1)*(180 - atan(zdis_ / xdis_) * 360 / 6.28);
				}
				else if (xdis_ < 0 && zdis_ < 0){
					xdis_ = (-1) * xdis_;
					zdis_ = (-1) * zdis_;
					preangle = (-1)*(atan(zdis_ / xdis_) * 360 / 6.28);
				}
				else{
					xdis_ = (-1)*xdis_;
					preangle = atan(zdis_ / xdis_) * 360 / 6.28;
				}

				preheightangle = atan(ydis_ / (sqrt(xdis_*xdis_ + zdis_*zdis_))) * 360 / 6.28;

				current->delta = (prelength-(TRAIN_DIS-predistance))/prelength;
				current->xaxis = cp.pos.x + (np.pos.x-cp.pos.x)* current->delta;	
				current->yaxis = cp.pos.y + (np.pos.y-cp.pos.y)* current->delta;
				current->zaxis = cp.pos.z + (np.pos.z-cp.pos.z)* current->delta;
				current->angle = preangle;
				current->heightangle = preheightangle;
				current->u = preu;		
				predistance = (prelength-(TRAIN_DIS-predistance));
				xo = (1 - current->delta)*porient_x + current->delta*norient_x;
				yo = (1 - current->delta)*porient_y + current->delta*norient_y;
				zo = (1 - current->delta)*porient_z + current->delta*norient_z;
				orient_length = sqrt(xo*xo+yo*yo+zo*zo);
				current->o_xo = zo;	current->o_yo = 0;	current->o_zo = -xo;
				current->o_angle = norm_angle;
			}
		}
			//if almost approach, set it to the next
		if (this->world.distance / length >= 0.99){
			this->world.distance = 0.0;
			this->world.trainU += 1;
		}
		if (this->world.trainU == this->world.points.size())
			this->world.trainU = 0;	

	}
	//cardinal cubic
	if(this->world.smoothness == 1)
	{

		//get the current point
		int i = int(this->world.trainU);
		int preu = i;
		int p;
		//P[0]
		if (i == 0) p = this->world.points.size() - 1;
		else p = i - 1;
		

		float p0x = this->world.points[p].pos.x;
		float p0y = this->world.points[p].pos.y;
		float p0z = this->world.points[p].pos.z;
		//p[1]
		float p1x = this->world.points[i].pos.x;
		float p1y = this->world.points[i].pos.y;
		float p1z = this->world.points[i].pos.z;

		float porient_x = this->world.points[i].orient.x;
		float porient_y = this->world.points[i].orient.y;
		float porient_z = this->world.points[i].orient.z;
			
		
		//P[2]
		p = (i + 1) % (world.points.size());
		float p2x = this->world.points[p].pos.x;
		float p2y = this->world.points[p].pos.y;
		float p2z = this->world.points[p].pos.z;
		float norient_x = this->world.points[p].orient.x;
		float norient_y = this->world.points[p].orient.y;
		float norient_z = this->world.points[p].orient.z;

		//P[3]
		p = (i + 2) % (world.points.size());
		float p3x = this->world.points[p].pos.x;
		float p3y = this->world.points[p].pos.y;
		float p3z = this->world.points[p].pos.z;

		//calculate distance between P1 and P2
		float length = sqrt((p1x - p2x)*(p1x - p2x) + (p1y - p2y)*(p1y - p2y) + (p1z - p2z)*(p1z - p2z));
		rate = 0.4;		
		if (!isarc){
			rate = length / strandlength;
		}

		if (this->trainCam->value()){
			speed /= 2;
		}
		//calculate the physics
		//parameter to set the offset of the speed when we use physics
		float phys;
		if (this->Physics->value()){
			//calculate the present Y-axis
			float py = this->world.distance;
			float preY = (((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*py*py*py + (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*py*py + ((-ts)*p0y + ts*p2y)*py + p1y);
			phys = this->physics->value() - 0.1 *(preY-5.f);
			
		}
		//no physics
		else{
			phys = this->physics->value();
		}
		
		//local parameter to get the date in the world->distance
		float t;
		t = this->world.distance;
		//boolean parameter to decide whether satisfy the requirement to go to the next point
		//only need to be calculated under arc-length seneriao
		float tangentx, tangenty, tangentz,tangentlength;
		//this is the speed vector
		tangentx = 3*((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*t*t + 2*(2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*t + ((-ts)*p0x + ts*p2x);
		tangenty = 3*((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*t*t + 2* (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*t + ((-ts)*p0y + ts*p2y);
		tangentz = 3*((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*t*t + 2* (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*t + ((-ts)*p0z + ts*p2z);
		tangentlength = sqrt(tangentx*tangentx + tangenty*tangenty + tangentz*tangentz);	
		if (isarc){
			//simulate on a very small distance
			//this is the speed 
			this->world.delta = (0.05*speed*rate/4)*phys/tangentlength*80;
			this->world.distance += this->world.delta;
		}
		else{
			this->world.delta = (0.05*speed*rate/2)*phys;
			this->world.distance += this->world.delta;
		}
		//calculate the matrix
		float tx, ty, tz;
		float rx, ry, rz;
		tx = (((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*t*t*t + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*t*t + ((-ts)*p0x + ts*p2x)*t + p1x);
		ty = (((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*t*t*t + (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*t*t + ((-ts)*p0y + ts*p2y)*t + p1y);
		tz = (((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*t*t*t + (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*t*t + ((-ts)*p0z + ts*p2z)*t + p1z);



		this->world.xaxis = tx;
		this->world.yaxis = ty;
		this->world.zaxis = tz;

		float xo = (1 - t)*porient_x + t*norient_x;
		float yo = (1 - t)*porient_y + t*norient_y;
		float zo = (1 - t)*porient_z + t*norient_z;
		float orient_length = sqrt(xo*xo+yo*yo+zo*zo);
		float norm_angle = acos(yo/orient_length) / PI_HERE * 180;
		this->world.txo = zo;	this->world.tyo = 0; this->world.tzo = -xo; this->world.or_angle = norm_angle;
		//parameter help to set trainCamera
		this->world.viewx = tx + 30 * tangentx;
		this->world.viewy = ty + 30 * tangenty;
		this->world.viewz = tz + 30 * tangentz;
		if (tangentx >= 0 && tangentz >= 0){
			this->world.angle = 180 - atan(tangentz / tangentx) * 360 / 6.28;
		}
		else if (tangentx > 0 && tangentz < 0){
			this->world.angle = (-1)*(180 - atan((-1)*tangentz / tangentx) * 360 / 6.28);
		}
		else if (tangentx < 0 && tangentz < 0){
			this->world.angle = (-1)*(atan(tangentz / tangentx) * 360 / 6.28);
		}
		else{
			this->world.angle = atan((-1)*tangentz / tangentx) * 360 / 6.28;
		}
		this->world.heightAngle = atan(tangenty / (sqrt(tangentx*tangentx + tangentz*tangentz))) * 180 / PI_HERE;
		float pred = t;
		for(int i=0;i<this->world.trainnum;i++)
		{
			TrainView::newcar *current = &this->trainView->newcars[i];
			float fixed_displacement = 0;
			//purpose: we want position of new car
			//calculate the new d. 
			//iteratively increase
			while(fixed_displacement<=TRAIN_DIS)
			{
				pred -= 0.01;
				//calibrate
				if(pred<=0)
				{
					//set it up.  
					pred = 1+pred;
					preu--;
					if(preu<0)	preu = this->world.points.size() - 1;
					
					if(preu==0)	p = this->world.points.size() -1;
					else p = preu-1;
					p0x = this->world.points[p].pos.x;
					p0y = this->world.points[p].pos.y;
					p0z = this->world.points[p].pos.z;
					
					p1x = this->world.points[preu].pos.x;
					p1y = this->world.points[preu].pos.y;
					p1z = this->world.points[preu].pos.z;
					porient_x = this->world.points[preu].orient.x;
					porient_y = this->world.points[preu].orient.y;
					porient_z = this->world.points[preu].orient.z;
											

					p = (preu + 1) % (world.points.size());
					p2x = this->world.points[p].pos.x;
					p2y = this->world.points[p].pos.y;
					p2z = this->world.points[p].pos.z;
					norient_x = this->world.points[p].orient.x;
					norient_y = this->world.points[p].orient.y;
					norient_z = this->world.points[p].orient.z;


					p = (preu + 2) % (world.points.size());
					p3x = this->world.points[p].pos.x;
					p3y = this->world.points[p].pos.y;
					p3z = this->world.points[p].pos.z;
				}
				//you would keep going down, until reach the points
				tangentx = 3*((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*pred*pred + 2*(2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*pred + ((-ts)*p0x + ts*p2x);
				tangenty = 3*((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*pred*pred + 2* (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*pred + ((-ts)*p0y + ts*p2y);
				tangentz = 3*((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*pred*pred + 2* (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*pred + ((-ts)*p0z + ts*p2z);
				tangentlength = sqrt(tangentx*tangentx + tangenty*tangenty + tangentz*tangentz);	
				fixed_displacement += tangentlength *0.01;
				//calculate some times
			}
			float xdis_ = tangentx, ydis_ = tangenty, zdis_=tangentz;
			float preangle;
			//calculate current coordinates
			if (xdis_ >= 0 && zdis_ >= 0){
					preangle = 180 - atan(zdis_ / xdis_) * 360 / 6.28;
			}
			else if (xdis_ > 0 && zdis_ < 0){
				zdis_ = (-1) * zdis_;
				preangle = (-1)*(180 - atan(zdis_ / xdis_) * 360 / 6.28);
			}
			else if (xdis_ < 0 && zdis_ < 0){
				xdis_ = (-1) * xdis_;
				zdis_ = (-1) * zdis_;
				preangle = (-1)*(atan(zdis_ / xdis_) * 360 / 6.28);
			}
			else{
				xdis_ = (-1)*xdis_;
				preangle = atan(zdis_ / xdis_) * 360 / 6.28;
			}

			//now, set up 
			current->angle = preangle;
			current->heightangle = atan(ydis_ / (sqrt(xdis_*xdis_ + zdis_*zdis_))) * 360 / 6.28;
			current->xaxis = (((-ts)*p0x + (2 - ts) * p1x + (ts - 2)*p2x + ts*p3x)*pred*pred*pred + (2 * ts * p0x + (ts - 3) * p1x + (3 - 2 * ts) * p2x - ts*p3x)*pred*pred + ((-ts)*p0x + ts*p2x)*pred + p1x);
			current->yaxis = (((-ts)*p0y + (2 - ts) * p1y + (ts - 2)*p2y + ts*p3y)*pred*pred*pred + (2 * ts * p0y + (ts - 3) * p1y + (3 - 2 * ts) * p2y - ts*p3y)*pred*pred + ((-ts)*p0y + ts*p2y)*pred + p1y);
			current->zaxis = (((-ts)*p0z + (2 - ts) * p1z + (ts - 2)*p2z + ts*p3z)*pred*pred*pred + (2 * ts * p0z + (ts - 3) * p1z + (3 - 2 * ts) * p2z - ts*p3z)*pred*pred + ((-ts)*p0z + ts*p2z)*pred + p1z);
			xo = (1 - pred)*porient_x + pred*norient_x;
			yo = (1 - pred)*porient_y + pred*norient_y;
			zo = (1 - pred)*porient_z + pred*norient_z;
			orient_length = sqrt(xo*xo+yo*yo+zo*zo);
			norm_angle = acos(yo/orient_length) / PI_HERE * 180;
			current->o_xo = zo;	
			current->o_yo = 0;	
			current->o_zo = -xo;
			current->o_angle = norm_angle;
		}
		if (t >= 1.0){
		this->world.distance -= 1.0;
		this->world.trainU += 1;	
		}

		if (this->world.trainU == world.points.size()) this->world.trainU = 0;
		//we have defined preu, pren before. 

	}	

}