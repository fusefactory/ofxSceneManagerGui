//
//  SceneGui.hpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 20/03/2020.
//

#ifndef SceneGui_hpp
#define SceneGui_hpp

#include <stdio.h>
#include "ofxPanel.h"
#include "ofxTransitionSlider.h"
#include "SceneBaseGui.hpp"

class SceneTimelineGui : public SceneBaseGui{

public:
    void load() override;
    ofxTextField                name;           //name human readable of current scene
    ofxToggle                   autoStart;      //when change scene autostart
    ofxToggle                   infinityTime;       //remain in this scene until the trigger signal
    ofxIntSlider                startTime;      //time to start scene in millis
    ofxFloatSlider              transitionTime;     //duration of transition
};

#endif /* SceneGui_hpp */
