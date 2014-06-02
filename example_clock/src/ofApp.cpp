#include "ofApp.h"

// example that apply force along the minute hand direction every minute
// click to pause/resume

// Naoto Hieda <micuat@gmail.com> 2014

//--------------------------------------------------------------
void ofApp::setup(){
	spidar.init();
	spidar.open();
	spidar.setForce(0, 0);
	ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){
	spidar.update();
	
	if( ofGetSeconds() == 0 && count < 0 && nowMinute != ofGetMinutes() ) {
		count = 0;
		nowMinute = ofGetMinutes();
	}
	if( count >= 0 && count < bins ) {
		float amp;
		if( count < bins/2 ) amp = -1.0;
		else amp = 1.0;
		float theta = ofMap(ofGetMinutes(), 0, 60, 0, TWO_PI);
		float vx = sin(theta);
		float vy = -cos(theta);
		if( abs(vx) < 0.001 ) vx = 0.0;
		if( abs(vy) < 0.001 ) vy = 0.0;
		if( !stopNow ) spidar.setForce(amp * vx, amp * vy, 40);
		count++;
		
		if( count == bins ) count = -1;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (!stopNow) {
		ofBackground(10, 240, 10);
	} else {
		ofBackground(240, 10, 10);
	}
	
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
    count = 0;
	stopNow = !stopNow;
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
