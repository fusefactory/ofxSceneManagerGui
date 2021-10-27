//
//  SceneGui.cpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 20/03/2020.
//

#include "SceneTimelineGui.hpp"

void SceneTimelineGui::load(){    
    add(name.setup(               "name", ""));
    add(startTime.setup(          "startTime", 0, 0, 3600000));
    add(autoStart.setup(          "autoStart", true));
    add(infinityTime.setup(       "infinityTime", false));
    add(transitionTime.setup(     "transitionTime", 0.0, 0.0, 120000.0));
}
