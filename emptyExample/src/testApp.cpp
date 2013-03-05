#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	spidar.init();
	spidar.open();
}

//--------------------------------------------------------------
void testApp::update(){
	spidar.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	
	spidar.draw(0xfefefe);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	spidar.setForce(-1.0, 0, 200);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	spidar.setForce(1.0, 1.0);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	spidar.setForce(0.0, 0.0);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}