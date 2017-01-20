#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    aruco_marker.load("images/aruco_34x34cm_ID00.jpg");
    ofBackground(255,255,255);
    ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    ofSetVerticalSync(true);
    client.connect("riker", 9090);
    client.addListener(this);
}

//--------------------------------------------------------------
void ofApp::update(){

//    if (marker_size < 100){
//        step = -step;
//    }
//    else if (marker_size > 300){
//        step = -step;
//    }
//    marker_size += step;
}

//--------------------------------------------------------------
void ofApp::draw(){
    int marker_size_pixels = marker_size / pixel_pitch;
    int posx = (ofGetWindowWidth()/2)-marker_size_pixels/2;
    int posy = (ofGetWindowHeight()/2)-marker_size_pixels/2;
    aruco_marker.draw(posx, posy, marker_size_pixels, marker_size_pixels);
}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    if ( !args.json.isNull() ){
        std::string op = args.json.get("op", "empty" ).asString();
        std::string topic = args.json.get("topic", "error").asString();
        //cout<<"op value: "<<op<<endl;

        if(op == "publish"){
            if(topic == "/marker_size" ){
                int requested_marker_size = args.json.get("msg", "error").get("data", "error").asInt();
                cout<<"Data value: "<<requested_marker_size<<endl;

                setMarquerSize(requested_marker_size);
                publishMarkerStatus("OK");
                draw();
            }
        }
    } else {
        cout<<"Received message with no JSON formatting"<<args.message<<endl;
    }
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofApp::topicSubscribe(std::string topic, std::string type){
    // To subscribe to a topic
    Json::Value msg;
    msg["op"] = "subscribe";
    msg["topic"] = topic;
    msg["type"] = type;
    client.send(msg.toStyledString());

    /*
    { "op": "subscribe",
      (optional) "id": <string>,
      "topic": <string>,
      (optional) "type": <string>,
      (optional) "throttle_rate": <int>,
      (optional) "queue_length": <int>,
      (optional) "fragment_size": <int>,
      (optional) "compression": <string>
    }
    */

}

//--------------------------------------------------------------
void ofApp::topicAdvertise(std::string topic, std::string type){
    // To advertise a topic
    Json::Value msg;
    msg["op"] = "advertise";
    msg["topic"] = topic;
    msg["type"] = type;
    client.send(msg.toStyledString());
}

//--------------------------------------------------------------
void ofApp::setMarquerSize(int requested_marker_size){
    float max_width_mm = pixel_pitch*ofGetWindowWidth();
    float max_height_mm = pixel_pitch*ofGetWindowHeight();
    float max_size = std::min(max_width_mm,max_height_mm)-10;

    //cout<<"Screen Width: "<< ofGetWindowWidth() <<endl;
    //cout<<"Screen Height: "<< ofGetWindowHeight() <<endl;
    //cout<<"Screen Width mm: "<< max_width_mm <<endl;
    //cout<<"Screen Height mm: "<< max_height_mm <<endl;


    if (requested_marker_size >= max_size){
        marker_size = max_size;
        cout<<"Error requested a Marker Size greater than maximum value"<< endl;
        cout<<"Setting the maximum"<< endl;
    } else if (requested_marker_size < 0.0){
        marker_size = 0.0;
        cout<<"Error requested a Marker Size smaller than minimum value"<< endl;
        cout<<"Setting the minimum"<< endl;
    } else {
        marker_size = requested_marker_size;
    }
    cout<<"New Marker Size: "<< requested_marker_size <<endl;
}

//--------------------------------------------------------------
void ofApp::publishMarkerStatus(std::string status){
    Json::Value pub_msg, msg;
    pub_msg["op"] = "publish";
    pub_msg["topic"] = "/dynamic_marker_status";
    pub_msg["type"] = "std_msgs/String";
    pub_msg["msg"] = msg;

    msg["data"] = status;

    pub_msg["msg"] = msg;

    client.send(pub_msg.toStyledString());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    topicSubscribe("/marker_size", "std_msgs/Int32");
    topicAdvertise("/dynamic_marker_status", "std_msgs/String");
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

