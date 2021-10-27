//
//  Scene.cpp
//  VanGoghInMe_OF2
//
//  Created by Matteo on 03/11/2020.
//

#include "Scene.hpp"

Scene::Scene(const Scene &scene){
    mainNumber = scene.mainNumber;
    subNumber = scene.subNumber;
    timelineFilename = scene.timelineFilename;
    audioFilename = scene.audioFilename;
    kinectFilename = scene.kinectFilename;
}
