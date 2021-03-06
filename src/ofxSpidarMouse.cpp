/*******************************************************
 SPIDAR-mouse-server for Linux, FreeBSD and Mac OS X
 Copyright (c) 2013 Naoto Hieda
 March 3, 2013 
 Licensed under MIT license.
 https://github.com/micuat/SPIDAR-mouse-UNIX
 
 ofxSpidarMouse.cpp
 
 ********************************************************/

#include "ofxSpidarMouse.h"
#include "hidapi.h"

ofxSpidarMouse::ofxSpidarMouse()
{
	isInitialized = false;
	isOpened = false;
}

int ofxSpidarMouse::init()
{
	isInitialized = false;
	
	int res;
	unsigned char buf[256];
#define MAX_STR 255
	wchar_t wstr[MAX_STR];

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("  Release:      %hx\n", cur_dev->release_number);
		printf("  Interface:    %d\n",  cur_dev->interface_number);
		printf("\n");
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
	
	// Set up the command buffer.
	memset(buf,0x00,sizeof(buf));
	buf[0] = 0x01;
	buf[1] = 0x81;
	
	
	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	handle = hid_open(0x4d8, 0x3f, NULL);
	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}
	
	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read manufacturer string\n");
	printf("Manufacturer String: %ls\n", wstr);
	
	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read product string\n");
	printf("Product String: %ls\n", wstr);
	
	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read serial number string\n");
	printf("Serial Number String: (%d) %ls", wstr[0], wstr);
	printf("\n");
	
	// Read Indexed String 1
	wstr[0] = 0x0000;
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read indexed string 1\n");
	printf("Indexed String 1: %ls\n", wstr);
	
	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);
	
	// Try to read from the device. There shoud be no
	// data here, but execution should not block.
	res = hid_read(handle, buf, 17);
	
	// Send a Feature Report to the device
	buf[0] = 0x2;
	buf[1] = 0xa0;
	buf[2] = 0x0a;
	buf[3] = 0x00;
	buf[4] = 0x00;
	res = hid_send_feature_report(handle, buf, 17);
	if (res < 0) {
		printf("Unable to send a feature report.\n");
	}
	
	memset(buf,0,sizeof(buf));
	
	// Read a Feature Report from the device
	buf[0] = 0x2;
	res = hid_get_feature_report(handle, buf, sizeof(buf));
	if (res < 0) {
		printf("Unable to get a feature report.\n");
		printf("%ls\n", hid_error(handle));
	}
	else {
		// Print out the returned buffer.
		printf("Feature Report\n   ");
		for (int i = 0; i < res; i++)
			printf("%02hhx ", buf[i]);
		printf("\n");
	}
	
	isInitialized = true;
	return 0;
}

ofxSpidarMouse::~ofxSpidarMouse()
{
	if( isOpened ) {
		close();
	}
	
	if( isInitialized ) {
		hid_close(handle);
		
		/* Free static HIDAPI objects. */
		hid_exit();	
	}
}

int ofxSpidarMouse::open()
{
	isOpened = false;
	
	if( !isInitialized ) {
		return 1;
	}
	
	int res;
	unsigned char buf[256];
	float conf_data[4];
	
	// Initialize parameters
	
	interG_X=0.0;
	interG_Y=0.0;
	SetForce_Mag=0.0;
	SetForce_Dirunit_X=0.0;
	SetForce_Dirunit_Y=0.0;
	Max_Force=1.5;
	defaultDuration = 4; // 4ms
	duration = 0;
	
	forceStack.clear();
	//////////////// Start SPIDAR-mouse
	
	memset(buf,0,sizeof(buf));
	
	buf[0] = 0x00;
	buf[1] = 0x82;
	res = hid_write(handle, buf, 65);
	if (res < 0) {
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 1;
	}
	
	// Read requested state. hid_read() has been set to be
	// non-blocking by the call to hid_set_nonblocking() above.
	// This loop demonstrates the non-blocking nature of hid_read().
	res = 0;
	while (res == 0) {
		res = hid_read(handle, buf, sizeof(buf));
		if (res == 0)
			printf("waiting...\n");
		if (res < 0)
			printf("Unable to read()\n");
#ifdef WIN32
		Sleep(500);
#else
		usleep(500*1000);
#endif
	}
	
	printf("Data read:\n   ");
	// Print out the returned buffer.
	for (int i = 0; i < res; i++)
		printf("%02hhx ", buf[i]);
	printf("\n");
	
	memset(buf,0,sizeof(buf));
	
	conf_data[0] = 0.3;
	conf_data[1] = 1.5;
	conf_data[2] = 47.9867;
	conf_data[3] = 13.1215;
	MinForce_Bytes = (unsigned char *)&conf_data[0];
	MaxForce_Bytes = (unsigned char *)&conf_data[1];
	Fun_a_Bytes = (unsigned char *)&conf_data[2];
	Fun_b_Bytes = (unsigned char *)&conf_data[3];
	
	buf[0] = 0;
	buf[1] = 0x80;
	buf[2] = MinForce_Bytes[0];
	buf[3] = MinForce_Bytes[1];
	buf[4] = MinForce_Bytes[2];
	buf[5] = MinForce_Bytes[3];
	
	buf[6] = MaxForce_Bytes[0];
	buf[7] = MaxForce_Bytes[1];
	buf[8] = MaxForce_Bytes[2];
	buf[9] = MaxForce_Bytes[3];
	
	buf[10] = Fun_a_Bytes[0];
	buf[11] = Fun_a_Bytes[1];
	buf[12] = Fun_a_Bytes[2];
	buf[13] = Fun_a_Bytes[3];
	
	buf[14] = Fun_b_Bytes[0];
	buf[15] = Fun_b_Bytes[1];
	buf[16] = Fun_b_Bytes[2];
	buf[17] = Fun_b_Bytes[3];
	res = hid_write(handle, buf, 65);
	if (res < 0) {
		printf("Unable to start SPIDAR-mouse: %ls\n", hid_error(handle));
		return 1;
	}
	
	setForce(0.0, 0.0);
	
	isOpened = true;
	return 0;
}

int ofxSpidarMouse::close()
{
	if( !isOpened ) {
		return 1;
	}
	
	int res;
	unsigned char buf[256];
	
	buf[0] = 0;
	buf[1] = 0x99;
	res = hid_write(handle, buf, 65);
	if (res < 0) {
		printf("Unable to close SPIDAR-mouse: %d %ls\n", res, hid_error(handle));
	}
		
	return 0;
}

int ofxSpidarMouse::update()
{
	unsigned long curTime = ofGetSystemTime();
	
	if( duration != 0 && (curTime - startTime) > duration ) {
		setForce(0.0, 0.0);
		duration = 0;
	}
	
	forceStack.push_back(ofVec2f(Force_XScale, Force_YScale));
	if( forceStack.size() > 30 ) {
		forceStack.erase(forceStack.begin());
	}
	
	return 0;
}

int ofxSpidarMouse::setForce(float fx, float fy)
{
	duration = 0; // infinite
	
	return sForce(fx, fy);
}

int ofxSpidarMouse::setForce(float fx, float fy, int d)
{
	startTime = ofGetSystemTime();
	
	if( d < defaultDuration ) {
		duration = defaultDuration;
	} else {
		duration = d;
	}
	
	return sForce(fx, fy);
}


int ofxSpidarMouse::sForce(float fx, float fy)
{
	if( fabsf(fx) > 1.0 ) {
		Force_XScale = fx / fabsf(fx);
	} else {
		Force_XScale = fx;
	}

	if( fabsf(fy) > 1.0 ) {
		Force_YScale = fy / fabsf(fy);
	} else {
		Force_YScale = fy;
	}
	
	if( !isOpened ) {
		return 1;
	}
	
	Force_X = Force_XScale * Max_Force;
	Force_Y = Force_YScale * Max_Force;
	SetForce_Mag = sqrt(Force_X * Force_X + Force_Y * Force_Y);
	if ( SetForce_Mag > 0.0 ) {
		SetForce_Dirunit_X = Force_X / SetForce_Mag;
		SetForce_Dirunit_Y = Force_Y / SetForce_Mag;
	}
		
	int res;
	unsigned char buf[256];

	memset(buf,0x00,sizeof(buf));
	
	buf[0] = 0;
	buf[1] = 0x97;		 
	interG_X_Bytes = (unsigned char *)&interG_X;
	interG_Y_Bytes = (unsigned char *)&interG_Y;
	Force_Mag_Bytes = (unsigned char *)&SetForce_Mag;
	Force_Dirunit_X_Bytes = (unsigned char *)&SetForce_Dirunit_X;
	Force_Dirunit_Y_Bytes = (unsigned char *)&SetForce_Dirunit_Y;
	
	buf[2] = interG_X_Bytes[0];
	buf[3] = interG_X_Bytes[1];
	buf[4] = interG_X_Bytes[2];
	buf[5] = interG_X_Bytes[3];
	
	buf[6] = interG_Y_Bytes[0];
	buf[7] = interG_Y_Bytes[1];
	buf[8] = interG_Y_Bytes[2];
	buf[9] = interG_Y_Bytes[3];
	
	buf[10] = Force_Mag_Bytes[0] ;
	buf[11] = Force_Mag_Bytes[1] ;
	buf[12] = Force_Mag_Bytes[2] ;
	buf[13] = Force_Mag_Bytes[3] ;
	
	buf[14] = Force_Dirunit_X_Bytes[0] ;
	buf[15] = Force_Dirunit_X_Bytes[1] ;
	buf[16] = Force_Dirunit_X_Bytes[2] ;
	buf[17] = Force_Dirunit_X_Bytes[3] ;
	
	buf[18] = Force_Dirunit_Y_Bytes[0] ;
	buf[19] = Force_Dirunit_Y_Bytes[1] ;
	buf[20] = Force_Dirunit_Y_Bytes[2] ;
	buf[21] = Force_Dirunit_Y_Bytes[3] ;
	res = hid_write(handle, buf, 65);
	if (res < 0) {
		printf("Unable to make feedback: %ls\n", hid_error(handle));
		return 1;
	}
	
	return 0;
}

void ofxSpidarMouse::draw(int col)
{
	ofVec2f circleCenter(80, 100);
	int r = 30;
	
	string info = "";
	info += "X Value: "+ofToString(Force_XScale, 3)+"\n";
	info += "Y Value: "+ofToString(Force_YScale, 3)+"\n\n";
 	ofSetHexColor(col);
	ofDrawBitmapString(info, 30, 30);
	
	ofFill();
	
	for( int i = 0 ; i < forceStack.size() ; i++ ) {
		ofSetColor(i * 6);
		if( fabsf(forceStack[i].x) > 0.01 || fabsf(forceStack[i].y) > 0.01 ) {
			ofPushMatrix();
			ofTranslate(circleCenter + forceStack[i] * r * i / 30);
			ofRotateZ(ofRadToDeg(atan2(forceStack[i].y, forceStack[i].x)));
			ofRect(-2, -4, 4, 8);
			ofPopMatrix();
		}
	}
	
	ofNoFill();
 	ofSetHexColor(col);
	ofCircle(circleCenter, r * 1.41);
	ofCircle(circleCenter, r * 1.41 / 2);
}