/*******************************************************
 SPIDAR-mouse-server for Linux, FreeBSD and Mac OS X
 Copyright (c) 2013 Naoto Hieda
 March 3, 2013 
 Licensed under MIT license.
 https://github.com/micuat/SPIDAR-mouse-UNIX
 
 ofxSpidarMouse.h
 
 ********************************************************/

#pragma once

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "hidapi.h"

#include "ofMain.h"

class ofxSpidarMouse {
	unsigned char *MinForce_Bytes, *MaxForce_Bytes, *Fun_a_Bytes, *Fun_b_Bytes;
	float interG_X;
	float interG_Y;
	float SetForce_Mag;
	float SetForce_Dirunit_X;
	float SetForce_Dirunit_Y;
	float Force_XScale, Force_YScale;
	float Force_X,Force_Y;
	float Max_Force;
	unsigned char *interG_X_Bytes;
	unsigned char *interG_Y_Bytes;
	unsigned char *Force_Mag_Bytes;
	unsigned char *Force_Dirunit_X_Bytes;
	unsigned char *Force_Dirunit_Y_Bytes;
	
	hid_device *handle;
	struct hid_device_info *devs, *cur_dev;
	
	unsigned long startTime;
	int defaultDuration;
	int duration;
	
	bool isInitialized, isOpened;

	int sForce(float, float);

public:
	ofxSpidarMouse();
	~ofxSpidarMouse();
	int init();
	int open();
	int close();
	int update();
	int setForce(float, float);
	int setForce(float, float, int);
	void draw(int);
};


