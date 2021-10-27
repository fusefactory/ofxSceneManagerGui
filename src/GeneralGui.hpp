//
//  GeneralGui.hpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 20/03/2020.
//

#ifndef GeneralGui_hpp
#define GeneralGui_hpp

#include <stdio.h>
#include "ofxPanel.h"

class GeneralGui : public ofxPanel{
    
public:
    void load();
    ofxToggle       play;               //button to play / pause
    ofxIntSlider    time;               //current time of the show
    ofxIntSlider    sceneTime;          //time relative to main scene
    ofxIntSlider    relSceneTime;       //rel time of current scene

    ofxLabel        timeHr;             //show time human readeble
    ofxLabel        relSceneTimeHr;      //rel scene time human readeble
    ofxLabel        sceneTimeHr;        //scene time human readeble
    ofxIntSlider    scene;              //index of scene
    ofxLabel        sceneFile;          //filename of current scene
    
    std::vector<ofxBaseGui*> additionalGui;
};

#endif /* GeneralGui_hpp */
