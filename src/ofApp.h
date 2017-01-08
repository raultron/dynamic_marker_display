#pragma once

#include "ofMain.h"

#define MARKER_BASE_SIZE 200;  //In milimeters
class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    ofImage aruco_marker;
    int marker_size = MARKER_BASE_SIZE;
    int monitor_width = 1920;
    int monitor_height = 1200;
    int step = 1;

};

