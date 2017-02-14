#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include <json/json.h>

#define MARKER_BASE_SIZE 0.15;  //In meters
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

    //marker related variables and methods
    enum marker_family_enum_ { whycon, aruco_single, aruco_multi, pitag};
    void set_marker(int marker_family, int marker_size, int marker_id, int message_id);
    void set_marker_response(bool result, std::string message, int message_id);

    ofImage aruco_single_ID88_;
    ofImage aruco_single_ID00_;
    ofImage aruco_multi_;
    ofImage whycon_ID00_;
    int marker_family_ = aruco_single;
    int marker_id_ = 00;
    int marker_size_ = MARKER_BASE_SIZE;
    //float pixel_pitch = 0.270; // in milimeters (RIKER) at max resolution
    float pixel_pitch = 0.1211; // in milimeters (ATLAS) at max resolution

    ofxLibwebsockets::Client client;
};



