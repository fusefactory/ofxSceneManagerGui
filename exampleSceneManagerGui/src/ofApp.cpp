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
    
    //set positions of other guis
    int guiWidth = 202;
    ofVec2f guiPos = ofVec2f(10 + guiWidth + 10, 30);
    
    updateGui.setPosition(guiPos.x, guiPos.y);
    drawGui.setPosition(guiPos.x + guiWidth, guiPos.y);
}

//--------------------------------------------------------------
void ofApp::update(){
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
