#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    aruco_single_ID88_.load("images/aruco_34x34cm_ID88.png");
    aruco_single_ID00_.load("images/aruco_34x34cm_ID00.jpg");
    aruco_multi_.load("images/aruco_multi.png");
    whycon_ID00_.load("images/whycon_outer150_inner70.png");
    ofBackground(255,255,255);
    ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    ofSetVerticalSync(true);
    client.connect("riker", 9090);
    client.addListener(this);
    topicSubscribe("/dynamic_marker/set_marker", "dynamic_marker/set_marker");
    topicAdvertise("/dynamic_marker/set_marker_response", "dynamic_marker/set_marker_response");
}

//--------------------------------------------------------------
void ofApp::update(){


}

//--------------------------------------------------------------
void ofApp::draw(){
    int marker_size_pixels = marker_size_ / pixel_pitch_mm_;
    int posx = (ofGetWindowWidth()/2)-marker_size_pixels/2;
    int posy = (ofGetWindowHeight()/2)-marker_size_pixels/2;

    // Decide which marker family image to draw and what ID:
    if (marker_family_ == whycon){

        whycon_ID00_.draw(posx, posy, marker_size_pixels, marker_size_pixels);

    } else if (marker_family_ == aruco_single){

        if(marker_id_ == 00){
            aruco_single_ID00_.draw(posx, posy, marker_size_pixels, marker_size_pixels);
        } else if(marker_id_ == 88){
            aruco_single_ID88_.draw(posx, posy, marker_size_pixels, marker_size_pixels);
        } else{
            cout<<"Error. Requested an undefined Marker ID for Aruco Single, using default (aruco single)"<< endl;
            aruco_single_ID88_.draw(posx, posy, marker_size_pixels, marker_size_pixels);
        }

    } else if (marker_family_ == aruco_multi){
        //float size_squares = 0.0698; //m
        int n_squares_x = 5 ;
        int n_squares_y = 5 ;
        float inter_square_sep = 20.0; // mm
        float s = marker_size_/(100.0);
        
        float total_width = n_squares_x*marker_size_+(n_squares_x-1)*inter_square_sep*s;
        float total_height = n_squares_y*marker_size_+(n_squares_y-1)*inter_square_sep*s;

        int total_width_px = total_width / pixel_pitch_mm_;
        int total_height_px = total_height / pixel_pitch_mm_;
        
        int posx = (ofGetWindowWidth()/2)-total_width_px/2;
        int posy = (ofGetWindowHeight()/2)-total_height_px/2;

        iters++;
        bool rotate_enabled = false;
        ofPushMatrix();
            ofTranslate((ofGetWindowWidth()/2), (ofGetWindowHeight()/2),0);
            if(rotate_enabled){
                ofRotate(iters*0.15); //! TODO add this as a paramater
            }
            ofPushMatrix();
                aruco_multi_.draw(-total_width_px/2, -total_height_px/2, total_width_px, total_height_px);
            ofPopMatrix();
        ofPopMatrix();
        //!TODO implement ID selection

    } else{
        cout<<"Error. Requested an undefined Marker Family, using default (aruco single)"<< endl;
        aruco_single_ID88_.draw(posx, posy, marker_size_pixels, marker_size_pixels);
    }

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

        if(op == "publish"){
            if(topic == "/dynamic_marker/set_marker" ){
                Json::Value msg = args.json.get("msg", "error");

                int marker_family = msg["marker_family"].asInt();
                double marker_size  = msg["marker_size"].asDouble()*1000; //from meters to milimiters
                int marker_id = msg["marker_id"].asInt();
                int message_id = msg["message_id"].asInt();

                cout<<"Setting the new marker config"<<endl;
                set_marker(marker_family, marker_size, marker_id, message_id);
            }
        }
    } else {
        cout<<"ERROR. Received a message with no JSON formatting"<<args.message<<endl;
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
void ofApp::set_marker(int marker_family, double marker_size, int marker_id, int message_id){
    bool result = true;
    std::string message = "OK";
    float max_width_mm = pixel_pitch_mm_*ofGetWindowWidth();
    float max_height_mm = pixel_pitch_mm_*ofGetWindowHeight();
    //cout<<"Screen Width: "<< ofGetWindowWidth() <<endl;
    //cout<<"Screen Height: "<< ofGetWindowHeight() <<endl;
    //cout<<"Screen Width mm: "<< max_width_mm <<endl;
    //cout<<"Screen Height mm: "<< max_height_mm <<endl;
    cout<<"Setting marker ID: "<< marker_id << endl;
    marker_id_ = marker_id;

    cout<<"Setting marker family: "<< marker_family << endl;
    marker_family_ = marker_family;

    cout<<"Setting the marker size: "<< marker_size << endl;
    //!TODO this 10 is an ugly hack to leave a white border around the marker (experiment different values)
    //! another idea is to make the marker inverted (black is now white and viceversa) so we take advantage
    //! of the black border of the laptop and the marker may be bigger. Using tud_img_prep is possible to
    //! invert the colors again


    float max_size = MARKER_BASE_SIZE + 0.001;
    if (marker_size >= max_size){
        marker_size = max_size;
        result = false;
        message = "Error. Requested a Marker Size greater than max value, Setting the max";
        cout<< message.c_str() << endl;
        //!TODO this should be sent back to decision_process in ROS
    } else if (marker_size < 0.0){
        marker_size = 0.0;
        result = false;
        message = "Error. Requested a Marker Size smaller than min value, Setting the min";
        cout<< message.c_str() << endl;
    }
    marker_size_ = marker_size;
    cout<<"New Marker Size: "<< marker_size_ <<endl;

    set_marker_response(result, message, message_id);
}

//--------------------------------------------------------------
void ofApp::set_marker_response(bool result, std::string message, int message_id){
    Json::Value pub_msg, msg;
    pub_msg["op"] = "publish";
    pub_msg["topic"] = "/dynamic_marker/set_marker_response";
    pub_msg["type"] = "/dynamic_marker/set_marker_response";

    msg["message_id"] = message_id;
    msg["result"] = result;
    msg["message"] = message;

    pub_msg["msg"] = msg;

    client.send(pub_msg.toStyledString());
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

