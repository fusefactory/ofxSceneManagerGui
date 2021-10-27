//
//  SceneManagerAudio.cpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 24/03/2020.
//

#include "SceneManagerAudio.hpp"
#include "ofxXmlSettings.h"
#include <thread>

bool mapContainsKey(std::map<string, ofSoundPlayer>& map, string key);

void SceneManagerAudio::setup(vector<reference_wrapper<SceneBaseGui>> &sceneGuiVector, ofVec2f &position){
    //personalize gui
    generalGui.additionalGui.push_back(audioTimeDiffMS.setup("audioTimeDiffMS", 0, 0, toleratedAudioDiffMS * 1.5f));
    
    ofxSceneManagerGui::setup(sceneGuiVector, position);
    
    bIsLoading = true;
    std::thread t1(&SceneManagerAudio::loadAudio, this);
    t1.detach();
    
    ofAddListener(ofxSceneManagerGui::onSceneChangedIndex,     this, &SceneManagerAudio::onSceneChangedIndex);
    ofAddListener(ofxSceneManagerGui::onSceneStarted,     this, &SceneManagerAudio::onSceneStarted);
    ofAddListener(ofxSceneManagerGui::onScenePaused,      this, &SceneManagerAudio::onScenePaused);
    ofAddListener(ofxSceneManagerGui::onSceneForwarded,   this, &SceneManagerAudio::onSceneForwarded);
    ofAddListener(ofxSceneManagerGui::onSceneBackwarded,  this, &SceneManagerAudio::onSceneBackwarded);
}

void SceneManagerAudio::loadAudio(){
    for(string timeline : timelines){
        string filename = sceneTimelineGui.getFolder() + "/" + timeline;
        ofxXmlSettings xml(filename);
        string audio = xml.getValue("timeline::audio", ""); //TODO: generalize name
        if(audio.size() > 1){
            ofSoundPlayer player;
            player.load(audio);
            ofLogVerbose() << "SceneManagerAudio:setup: loaded player from file: " << audio;
            //player.setVolume(0);
            timelineAudioMap.insert(pair<string, ofSoundPlayer>(timeline, player));
        }
    }
    
    bIsLoading = false;
    cout << "----------- AUDIO LOADED -----------" << endl;
}

void SceneManagerAudio::onSceneChangedIndex(int &index){
    string timeline = timelines.at(index);
    bool contaisAudio = mapContainsKey(timelineAudioMap, timeline);
    if(contaisAudio){
        if(currentSoundPlayer != NULL){
            currentSoundPlayer->stop();
        }
        
        ofLogVerbose() << "SceneMangerAudio::onSceneChangedIndex: " << timeline;
        currentSoundPlayer = &timelineAudioMap.find(timeline)->second;
        
        if(isPlaying()){
            currentSoundPlayer->play();
        }
    }else{
        if(currentSoundPlayer != NULL){
            if(! currentSoundPlayer->isPlaying()) currentSoundPlayer->play();
            
            syncPlayer();
            
            currentSoundPlayer->setPaused(!generalGui.play);
        }
    }
}

void SceneManagerAudio::onSceneStarted(){
    if(currentSoundPlayer != NULL){
        if(! currentSoundPlayer->isPlaying()) currentSoundPlayer->play();
        
        currentSoundPlayer->setPositionMS(generalGui.sceneTime);
        currentSoundPlayer->setPaused(false);
    }
}

void SceneManagerAudio::onScenePaused(){
    if(currentSoundPlayer != NULL){
        currentSoundPlayer->setPaused(true);
    }
}

void SceneManagerAudio::onSceneBackwarded(){
    if(currentSoundPlayer != NULL){
        currentSoundPlayer->setPositionMS(generalGui.sceneTime);
    }
}

void SceneManagerAudio::onSceneForwarded(){
    if(currentSoundPlayer != NULL){
        currentSoundPlayer->setPositionMS(generalGui.sceneTime);
    }
}

void SceneManagerAudio::update() {
    ofxSceneManagerGui::update();
    
    if(currentSoundPlayer != NULL){
        int diffMS = fabs(currentSoundPlayer->getPositionMS() - generalGui.sceneTime);
        audioTimeDiffMS = diffMS;
    }
}

void SceneManagerAudio::syncPlayer(){
    int diffMS = fabs(currentSoundPlayer->getPositionMS() - generalGui.sceneTime);
    
    ofLogVerbose() << "SceneMangerAudio::onSceneChangedIndex:AudioDiff" << diffMS;

    if(diffMS > toleratedAudioDiffMS) currentSoundPlayer->setPositionMS(generalGui.sceneTime);
}

bool mapContainsKey(std::map<string, ofSoundPlayer>& map, string key){
  if (map.find(key) == map.end()) return false;
  return true;
}
