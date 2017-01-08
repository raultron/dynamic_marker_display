#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    aruco_marker.load("images/aruco_34x34cm_ID00.jpg");
    ofBackground(0,0,0);
    ofSetFrameRate(240); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    ofSetVerticalSync(false);

}

//--------------------------------------------------------------
void ofApp::update(){

    if (marker_size < 100){
        step = -step;
    }
    else if (marker_size > 300){
        step = -step;
    }
    marker_size += step;
}

//--------------------------------------------------------------
void ofApp::draw(){
    float pixel_pitch = 0.270; // in milimeters
    int marker_size_pixels = marker_size / pixel_pitch;
    int posx = (monitor_width/2)-marker_size_pixels/2;
    int posy = (monitor_height/2)-marker_size_pixels/2;


    aruco_marker.draw(posx, posy, marker_size_pixels, marker_size_pixels);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//    float pixel_pitch = 0.270; // in milimeters
//    // update mouse x and y percent when the mouse moves
//    float mouseXPercent = float(x) / ofGetWidth();
//    float mouseYPercent = float(y) / ofGetHeight();
//    marker_size = mouseXPercent*MARKER_BASE_SIZE;
//    //std::cout << mouseXPercent << std::endl;;

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
