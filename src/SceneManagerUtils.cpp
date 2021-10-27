//
//  SceneManagerUtils.cpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 25/03/2020.
//

#include "SceneManagerUtils.hpp"

SceneManagerUtils::SceneManagerUtils(){
    mainSceneTerminalString = "00.xml";
}

bool SceneManagerUtils::isMainScene(const string filenameXml) const{
    return filenameXml.substr(filenameXml.length() - mainSceneTerminalString.length()) == mainSceneTerminalString;
}

string SceneManagerUtils::getMainSceneName(const string filenameXml) const{
    string mainSceneXml = filenameXml;
    mainSceneXml.resize(mainSceneXml.size() - mainSceneTerminalString.length());
    mainSceneXml += mainSceneTerminalString;
    
    return  mainSceneXml;
}

string SceneManagerUtils::getNextMainSceneName(const string filenameXml) const{
    if(filenameXml.size() > 1){
        string baseSceneName = filenameXml;
        string subString = baseSceneName.substr(8, 2);
        int i = stoi(subString);
        i++;
            
        return "timeline" + ofToString(i, 2, '0') + "_00.xml";
    }

    return "";
}

string SceneManagerUtils::getShiftedFilename(const string filename, const int increment) const{
    string baseSceneName = filename;
    
    baseSceneName.resize(baseSceneName.size()- mainSceneTerminalString.length());
    
    string name = "timeline00_";
    string subString = filename.substr(name.size(), 2);
    int i = stoi(subString);
    i += increment;
    string newFilename = baseSceneName + ofToString(i, 2, '0') + ".xml";
    return newFilename;
}

int SceneManagerUtils::getNumberMainScene(const string timelineFilename) const{
    string testString = "timelineXX_XX.xml";
    if(testString.length() == timelineFilename.length()){
        string s = timelineFilename;
        string name = "timeline";
        
        string nS = s.substr(name.size(), 2);
        
        int n = stoi(nS);
        
        return n;
    }
    
    return -1;
}


int SceneManagerUtils::getNumberSubScene(const string timelineFilename) const{
    string testString = "timelineXX_XX.xml";
    if(testString.length() == timelineFilename.length()){
        string s = timelineFilename;
        string name = "timelineXX_";
        
        string nS = s.substr(name.size(), 2);
        
        int n = stoi(nS);
        
        return n;
    }
    
    return -1;
}
