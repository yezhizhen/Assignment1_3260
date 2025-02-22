#pragma once
//  Train Project - 
// TrainWindow class description
//
// this class defines the window in which the project runs - its the
// outer windows that contain all of the widgets, including the "TrainView"
// which has the actual OpenGL window in which the train is drawn
//
// You might want to modify this class to add new widgets for controlling
// your train
//
// This takes care of lots of things - including installing itself into the
// FlTk "idle" loop so that we get periodic updates (if we're running the train).
//
// Written by Mike Gleicher, October 2008

//
// since this is an FLTk Window, we need to include FlTk Headers
// the pragmas turn off the warnings from FlTk - our code should have no warnings,
// but their code doesn't meet that standard
#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <Fl/Fl_Double_Window.h>
#include <Fl/Fl_Button.h>
#include <Fl/Fl_Group.H>
#include <Fl/Fl_Value_Slider.H>
#include <Fl/Fl_Browser.H>
#pragma warning(pop)

// we need to know what is in the world to show
#include "World.H"

// other things we just deal with as pointers, to avoid circular references
class TrainView;

class TrainWindow : public Fl_Double_Window {
public:
	TrainWindow(const int x=50, const int y=50);

	// keep track of the stuff in the world
	World world;

	// the widgets that make up the Window
	TrainView* trainView;

	Fl_Group* widgets;	// all widgets, grouped for resizing ease

	// utility buttons
	Fl_Button* resetButton;

	// which viewpoint are we drawing from
	Fl_Button* worldCam;
	Fl_Button* trainCam;
	Fl_Button* topCam;

	// the type of the spline (use its value to determine)
	Fl_Browser* splineBrowser;
	// the type of the car
	Fl_Browser* carBrowser;


	// are we animating the train?
	Fl_Button* runButton;
	// if we're animating it, how fast should it go?
	Fl_Value_Slider* speed;
	Fl_Button* arcLength;		// do we use arc length for speed?
	//controlling the curve tension
	Fl_Value_Slider* tension;
	//controlling the curve tension
	Fl_Value_Slider* physics;
	Fl_Value_Slider* FOG_SLIDER;
	Fl_Button* Physics;
	Fl_Button* Fog;	
	// call this method when things change
	void damageMe();

	// this moves the train forward on the track - its up to you to do this
	// correctly. it gets called from the idle callback loop
	// it should handle forward and backwards
	void advanceTrain(float dir = 1);

	// simple helper function to set up a button
	void togglify(Fl_Button*, int state=0);



};

// This code tells us where the original came from in CVS
// Its a good idea to leave it as-is so we know what version of
// things you started with
// $Header: /p/course/-gleicher/private/CVS/TrainFiles/TrainWindow.H,v 1.7 2008/10/19 01:54:28 gleicher Exp $
