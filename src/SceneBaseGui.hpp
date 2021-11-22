//
//  SceneBaseGui.hpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 20/03/2020.
//

#ifndef SceneBaseGui_hpp
#define SceneBaseGui_hpp

#include <stdio.h>
#include <string>
#include "ofxPanel.h"
#include "ofxTransitionSlider.h"

class SceneBaseGui : public ofxPanel{
public:
    virtual void load() {};
    virtual void update();
    
    void setFolder(std::string folder){SceneBaseGui::folder = folder ; };
    std::string getFolder(){return folder;};
    
    void setDurationAllTransitionSlider(float duration);
    void jumpToFinalValueTranistionSlider();
    void setFilename(std::string filename);
    std::string getFilename(){return filename; } ;
    void setTransitionTimeBased(bool timeBased, int framerateReference = 60);
    
protected:
    //TODO: improve to generics type
    std::vector<std::reference_wrapper<ofxTransitionFloatSlider>> transitionFloatSliderVector;  //put here all float transition slider
    std::vector<std::reference_wrapper<ofxTransitionIntSlider>>   transitionIntSliderVector;    //put here all int transition slider

private:
    std::string folder = "";
};

#endif /* SceneBaseGui_hpp */
