#pragma once

#include "ofMain.h"
#include "ofxSceneManagerGui.hpp"
#include "GuiWrap.h"
#include "ofxLTC.h"

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
    
        uint64_t getMillisFromTimeCode(int hours, int minutes, int seconds, int frames, int framerate);
    
        ofxSceneManagerGui sceneManager;
        UpdateGui updateGui;
        DrawGui drawGui;
        
        int count = 0;
        uint64_t timecodeMillis = 0;
        ofxLTCReceiver receiver;
        ofxLTCTimecode timecode;
        ofThreadChannel<ofxLTCTimecode> received_timecodes;
};
