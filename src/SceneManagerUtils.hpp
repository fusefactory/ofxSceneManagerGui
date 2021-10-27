//
//  SceneManagerUtils.hpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 25/03/2020.
//

#ifndef SceneManagerUtils_hpp
#define SceneManagerUtils_hpp

#include <stdio.h>
#include "ofMain.h"

class SceneManagerUtils{
    
public:
    SceneManagerUtils();
    bool isMainScene(const string filenameXml) const;
    string getMainSceneName(const string filenameXml) const;
    string getNextMainSceneName(const string filenameXml) const;
    string getShiftedFilename(const string filename, const int increment) const;
    int getNumberMainScene(const string timelineFilename) const;
    int getNumberSubScene(const string timelineFilename) const;

protected:
    string mainSceneTerminalString;
    
};

#endif /* SceneManagerUtils_hpp */
