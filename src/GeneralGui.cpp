//
//  GeneralGui.cpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 20/03/2020.
//

#include "GeneralGui.hpp"

void GeneralGui::load(){
    add(play.setup(              "play/stop", false));
    add(time.setup(              "time", 0, 0, 600));
    add(sceneTime.setup(         "sceneTime", 0, 0, 600));
    add(relSceneTime.setup(      "relSceneTime", 0, 0, 600));
    add(timeHr.setup(            "timeHr        ", ""));
    add(sceneTimeHr.setup(       "sceneTimeHr   ", ""));
    add(relSceneTimeHr.setup(    "relSceneTimeHr", ""));
    add(scene.setup(             "scene", 0, 0, 10)); //-1
    add(sceneFile.setup(         "", ""));
    
    for(ofxBaseGui *baseGui : additionalGui){
        add(baseGui);
    }
}
