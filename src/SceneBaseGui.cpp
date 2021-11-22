//
//  SceneBaseGui.cpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 20/03/2020.
//


#ifndef SceneBaseGui_cpp
#define SceneBaseGui_cpp

#include "SceneBaseGui.hpp"

void SceneBaseGui::setDurationAllTransitionSlider(float duration){
    for(ofxTransitionFloatSlider &slider : transitionFloatSliderVector){
        slider.setTransitionDuration(duration);
    }
    
    for(ofxTransitionIntSlider &slider : transitionIntSliderVector){
        slider.setTransitionDuration(duration);
    }
}

void SceneBaseGui::jumpToFinalValueTranistionSlider(){
    for(ofxTransitionFloatSlider &slider : transitionFloatSliderVector){
        slider.jumpToFinalValue();
    }
    
    for(ofxTransitionIntSlider &slider : transitionIntSliderVector){
        slider.jumpToFinalValue();
    }
}

void SceneBaseGui::update(){
    for(ofxTransitionFloatSlider &slider : transitionFloatSliderVector){
        slider.update();
    }
    
    for(ofxTransitionIntSlider &slider : transitionIntSliderVector){
        slider.update();
    }
}

void SceneBaseGui::setFilename(std::string filename){
    SceneBaseGui::filename = filename;
}

void SceneBaseGui::setTransitionTimeBased(bool timeBased, int framerateReference){
    for(ofxTransitionFloatSlider &slider : transitionFloatSliderVector){
        if(timeBased){
            slider.setTransitionTimeBased(true);
        }else{
            slider.setTransitionTimeBased(false);
            slider.setFramerateReference(framerateReference);
        }
    }
    
    for(ofxTransitionIntSlider &slider : transitionIntSliderVector){
        if(timeBased){
            slider.setTransitionTimeBased(true);
        }else{
            slider.setTransitionTimeBased(false);
            slider.setFramerateReference(framerateReference);
        }
    }
}

#endif /* SceneBaseGui_cpp */
