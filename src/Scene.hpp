//
//  Scene.hpp
//  VanGoghInMe_OF2
//
//  Created by Matteo on 03/11/2020.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "ofMain.h"

class Scene{
public:
    Scene(){};
    Scene(const Scene &scene);      //copy constructor
    int mainNumber;
    int subNumber;
    string timelineFilename;
    string audioFilename;
    string kinectFilename;
};

#endif /* Scene_hpp */
