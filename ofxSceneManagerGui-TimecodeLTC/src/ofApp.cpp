#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //1. Create a vector of SceneBase Gui
    vector<reference_wrapper<SceneBaseGui>> sceneGuiVector;
    
    //2. push_back your guis
    sceneGuiVector.push_back(updateGui);
    sceneGuiVector.push_back(drawGui);
    
    //3. get scene folder. Default is /scenes
    const string scenesDir = sceneManager.getSceneFolder();
    ofDirectory dir(ofToDataPath(scenesDir));
    
    //4. for each gui (child of SceneBaseGui) do the follow step
    //setup with name
    //set folder name
    //load
    //TODO: maybe can be improved
    updateGui.setup("UPDATE");
    updateGui.setFolder(dir.getOriginalDirectory() + "updateGui");
    updateGui.load();
    
    drawGui.setup("DRAW");
    drawGui.setFolder(dir.getOriginalDirectory() + "drawGui");
    drawGui.load();
    
    //5. Setup sceneManager with the vector of SceneBase and the position
    ofVec2f sceneManagerGuiPos = ofVec2f(10, 30);
    sceneManager.setup(sceneGuiVector, sceneManagerGuiPos);
    sceneManager.setExternalTime(&timecodeMillis);          //we choose to use an external time millis
    
    //set positions of other guis
    int guiWidth = 202;
    ofVec2f guiPos = ofVec2f(10 + guiWidth + 10, 30);
    
    updateGui.setPosition(guiPos.x, guiPos.y);
    drawGui.setPosition(guiPos.x + guiWidth, guiPos.y);
    
    //setup ofxLTC
    auto &&list = receiver.getDeivceList();
    for(auto i = 0; i < list.size(); ++i) {
        const auto &device = list[i];
        ofLogNotice() << i << ": " << device.name << " " << device.inputChannels;
    }
    auto &target_device = list[4]; // select device id what you need to use
    ofSoundStreamSettings setting;
    setting.setInDevice(target_device);
    setting.numInputChannels = target_device.inputChannels;
    setting.sampleRate = target_device.sampleRates.front();
    receiver.setup(setting);
    receiver.onReceive([=](ofxLTCTimecode code) {
        received_timecodes.send(code);
    });
}

//--------------------------------------------------------------
void ofApp::update(){
    while(!received_timecodes.empty()) {
        received_timecodes.receive(timecode);
    }
    
    if(timecode.day == 0){
        timecodeMillis = getMillisFromTimeCode(timecode.hour, timecode.min, timecode.sec, timecode.frame, 30);
    }
    
//    if(timecode.day != 0)
//        cout << timecode.toString() << endl;

    sceneManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    sceneManager.draw();
    updateGui.draw();
    drawGui.draw();
    
    string help =   "spacebar: start/pause\n";
    help +=         "key up - next scene\n";
    help +=         "key down - previous scene\n";
    help +=         "key right - go forward 2000 ms\n";
    help +=         "key left -  go back 2000 ms\n";
    help +=         "A - Add a scene\n";
    help +=         "S - Save current scene\n";
    help +=         "D - Delete current scene\n";
    
    ofDrawBitmapStringHighlight(help, 225, 250, ofColor(0, 127), ofColor(255));
    ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate(), 3), 10, 20, ofColor(0, 127), ofColor(255));
    ofDrawBitmapStringHighlight("LTC: " + timecode.toString(), 120, 20, ofColor(0, 127), ofColor(255));
    ofDrawBitmapStringHighlight("LTC ms: " + ofToString(timecodeMillis), 400, 20, ofColor(0, 127), ofColor(255));
}

uint64_t ofApp::getMillisFromTimeCode(int hours, int minutes, int seconds, int frames, int framerate){
    uint64_t millis = (1.0f * frames) / framerate * 1000.0f;    //millis
    millis += seconds * 1000.f;                                 // add seconds
    millis += minutes * 60.0f * 1000.0f;                        // add minutes
    millis += hours * 60.0f * 60.0f * 1000.0f;                  // add hours
    return millis;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_DOWN) {
        sceneManager.forcePrevScene();
        cout << "SCENE MANAGER GENERAL TIME: " << sceneManager.getGeneralGui().time << endl;
    }
    else if (key == OF_KEY_UP) {
        sceneManager.forceNextScene();
        cout << "SCENE MANAGER GENERAL TIME: " << sceneManager.getGeneralGui().time << endl;
    }
    else if (key == OF_KEY_LEFT) {
        sceneManager.forceBackwardTimeMillis(2000);
    }
    else if (key == OF_KEY_RIGHT) {
        sceneManager.forceForwardTimeMillis(2000);
    }
    else if (key == ' ') {
        sceneManager.getGeneralGui().play = !sceneManager.getGeneralGui().play;
    }
    
    if(key == 'a'){
        updateGui.rotation = 0.5;
    }
    if(key == 'b'){
        updateGui.rotation.jumpToValue(0.5);
    }
    if(key == 'c'){
        if(sceneManager.isTimeBased()) sceneManager.setFrameBased(60);
        else sceneManager.setTimeBased();
    }
    if(key == 'd'){
        if(sceneManager.getTimePlayingType() == ofSystemTimeMillis) sceneManager.setFrameBased(60);
        else sceneManager.setTimeBased();
    }
    
    if(key == 'A'){
        sceneManager.createNewFileSceneTimelineGui();
        ofSystemAlertDialog("SceneManager::Sub scene added");
    }
    if(key == 'S') {
        bool s = sceneManager.saveOverrideFileCurrentSceneGui();
        string name = sceneManager.getSceneTimelineGui().name;
        string message =  s == true ? "SceneManager::Scene Save " + name : "Error! The scene wasn't saved!";
        ofSystemAlertDialog(message);
    }
    if(key == 'D') {
        sceneManager.getGeneralGui().play = false;
        
        string name = sceneManager.getSceneTimelineGui().name;
        string response = ofSystemTextBoxDialog("Delete Scene", "Do you want delete current scene " + name + "?");
        if(response.size() > 1){
            sceneManager.deleteFileCurrentSceneTimelineGui();
        }
    }
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
