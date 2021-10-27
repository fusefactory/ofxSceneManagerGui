//
//  SceneManagerAudio.hpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 24/03/2020.
//

#ifndef SceneManagerAudio_hpp
#define SceneManagerAudio_hpp

#include <stdio.h>
#include "ofxSceneManagerGui.hpp"

class SceneManagerAudio : public ofxSceneManagerGui{
public:
    void setup(vector<reference_wrapper<SceneBaseGui>> &sceneGuiVector, ofVec2f &position);
    void update();

protected:
    void loadAudio();
    void onSceneChangedIndex(int &index);
    void onSceneStarted();
    void onScenePaused();
    void onSceneBackwarded();
    void onSceneForwarded();
    void syncPlayer();

    ofSoundPlayer *currentSoundPlayer;
    map<string, ofSoundPlayer> timelineAudioMap;
    
    ofxIntSlider audioTimeDiffMS;
    const int toleratedAudioDiffMS = 20;                //MS diff from time scene and audio time tollerated
};

#endif /* SceneManagerAudio_hpp */
