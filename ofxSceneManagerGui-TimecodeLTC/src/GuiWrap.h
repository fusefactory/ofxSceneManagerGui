#ifndef GuiWrap_h
#define GuiWrap_h

#include <stdio.h>
#include "ofxGui.h"
#include "SceneBaseGui.hpp"

class UpdateGui : public SceneBaseGui {
    
public:
    ofxTransitionFloatSlider velEnable;
    ofxTransitionIntSlider boundaries;
    ofxTransitionFloatSlider rotation;

    void load() override {
        add(velEnable.setup("velEnable", 0.0, 0.0, 20.0));
        add(boundaries.setup("boundaries", 0, 0, 100));
        add(rotation.setup("rotation", 1.0, 0.0, 1.0));

        //every ofxTransitionSlider needs to be added to transitionFloatSliderVector
        transitionIntSliderVector.push_back(boundaries);
        
        transitionFloatSliderVector.push_back(velEnable);
        transitionFloatSliderVector.push_back(rotation);
    }
};

class DrawGui : public SceneBaseGui {

public:
    ofxTransitionFloatSlider generalAlpha;
    ofxFloatSlider particleAlphaSR;
    ofxFloatSlider cameraRotation;

    void load() override {
        add(generalAlpha.setup("generalAlpha", 0.5, 0.0, 1.0));
        add(particleAlphaSR.setup("particleAlphaSR", 1.0, 0.0, 1.0));
        add(cameraRotation.setup("cameraRotation", 0.0, -1.0, 1.0));
        
        transitionFloatSliderVector.push_back(generalAlpha);
    }
};



#endif
