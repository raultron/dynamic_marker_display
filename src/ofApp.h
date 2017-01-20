#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include <json/json.h>

#define MARKER_BASE_SIZE 150;  //In milimeters
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

    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );

    //ROS related methods
    void topicSubscribe(std::string topic, std::string type);
    void topicAdvertise(std::string topic, std::string type);
    void publishMarkerStatus(std::string status);

    //marker related variables and methods
    void setMarquerSize(int requested_marker_size);

    ofImage aruco_marker;
    float marker_size = MARKER_BASE_SIZE;
    //float pixel_pitch = 0.270; // in milimeters (RIKER) at max resolution
    float pixel_pitch = 0.1211; // in milimeters (ATLAS) at max resolution

    ofxLibwebsockets::Client client;



};

