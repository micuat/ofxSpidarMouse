#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// initialization
	spidar.init();
	spidar.open();
}

//--------------------------------------------------------------
void ofApp::update(){
	// drain mouse to the center
	float fx = ofMap(mouseX, 0, ofGetWindowWidth(), 1, -1);
	float fy = ofMap(mouseY, 0, ofGetWindowHeight(), 1, -1);
	
	// use sqrt to magnify
	if( fx > 0 )
		fx = sqrt(abs(fx));
	else
		fx = -sqrt(abs(fx));
	
	if( fy > 0 )
		fy = sqrt(abs(fy));
	else
		fy = -sqrt(abs(fy));
	
	// set force
	spidar.setForce(fx, fy);
	
	spidar.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(200);
	
	ofPushMatrix();
	ofPushStyle();
	
	ofTranslate(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2);
	ofSetCircleResolution(32);
	float diff = ofGetElapsedTimef() - floor(ofGetElapsedTimef());
	for( int i = 16; i > 0; i-- ) {
		ofFill();
		ofSetColor(ofMap(i, 0, 16, 50, 200));
		ofCircle(0, 0, ofMap(i - diff, 0, 16, 0, ofGetWindowWidth() / 2));
	}
	
	ofPopStyle();
	ofPopMatrix();
	
	spidar.draw(0xfefefe);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
