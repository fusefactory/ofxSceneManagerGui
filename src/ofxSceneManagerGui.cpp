//
//  SceneManager.cpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 20/03/2020.
//

#include "ofxSceneManagerGui.hpp"
#include "ofxXmlSettings.h"
#include <regex>
#include <algorithm>

string format_duration( std::chrono::milliseconds ms );

ofxSceneManagerGui::ofxSceneManagerGui() {

}

ofxSceneManagerGui::ofxSceneManagerGui(SceneManagerSettings &sceneManagerSettings) : settings(sceneManagerSettings){
    
}

void ofxSceneManagerGui::setup(vector<reference_wrapper<SceneBaseGui>> &sceneGuiVector, ofVec2f &position){
    ofxSceneManagerGui::sceneGuiVector = sceneGuiVector;
    
    string folder = getSceneFolder();
    ofDirectory dir(folder);

    //GENERAL GUI
    generalGui.setup("GENERAL", "guiGeneral.xml", position.x, position.y);
    generalGui.load();
    
    //SCENE TIMELINE
    sceneTimelineGui.setup("timeline");            //name inside xml
    sceneTimelineGui.setFolder(dir.getOriginalDirectory() + "_timeline");
    sceneTimelineGui.load();
    sceneTimelineGui.setPosition(generalGui.getPosition().x, generalGui.getPosition().y + generalGui.getHeight() + 20);

    reloadTimelines(sceneTimelineGui.getFolder());
        
    string lastPath = retriveXmlPath(sceneTimelineGui, getNumberScenes()-1);
    generalGui.time.setMax(getStartTime(lastPath));
    generalGui.scene.setMax(timelines.size());
    generalGui.relSceneTime.setMin(0);

    changeScene(0);
}

void ofxSceneManagerGui::reloadTimelines(string folder){
    ofDirectory timelineFolder = ofDirectory(folder);
    timelineFolder.allowExt("xml");
    timelineFolder.listDir();
    timelineFolder.sort();
    
    timelines.clear();
    for(int i = 0; i < timelineFolder.size(); i++){
        string path = timelineFolder.getName(i);
        ofLogVerbose() << "ofxSceneManagerGui::setup: found timeline file: " << path;;
        timelines.push_back(path);
    }
}

string ofxSceneManagerGui::retriveXmlPath(SceneBaseGui &sceneBaseGui, int index){
    if(index < timelines.size())
        return sceneBaseGui.getFolder() + "/" + timelines.at(index);
    
    return "";
}

int ofxSceneManagerGui::getStartTime(string fileXml){
    ofxXmlSettings xml;
    xml.load(fileXml);
    
    int startTime =  xml.getValue(sceneTimelineGui.getName() +"::"+ sceneTimelineGui.startTime.getName(), -1);
    
    if(startTime >= 0){
        if(! utils.isMainScene(fileXml)){
            string mainSceneXml = utils.getMainSceneName(fileXml);
            int addTime = getStartTime(mainSceneXml);
            
            return startTime + addTime;
        }
    }
    
    return startTime;
}

int ofxSceneManagerGui::getNumberCurrentMainScene(){
    string s = generalGui.sceneFile;
    return utils.getNumberMainScene(generalGui.sceneFile);
}

int ofxSceneManagerGui::getNumberCurrentSubScene() {
    string s = generalGui.sceneFile;
    return utils.getNumberSubScene(generalGui.sceneFile);
}

string ofxSceneManagerGui::getCurrentSceneName() {
   return sceneNames.at(generalGui.scene);
}

vector<string> ofxSceneManagerGui::getNextScenesNames(int number) {
    vector<string> nextNames;
    int currentMainScene = getNumberCurrentMainScene();

    for (int i = 1; i <= number; i++) {
        int index = generalGui.scene + i;
        if (index < sceneNames.size()) {
            string sceneFile = timelines.at(index);
            int mainScene = utils.getNumberMainScene(sceneFile);
            if (mainScene == currentMainScene) {
                string name = sceneNames.at(index); //name of the current scene
                nextNames.push_back(name);
            }
            else {
                break;
            }
        }
    }

    return nextNames;
}

vector<string> ofxSceneManagerGui::getPreviousScenesNames(int number) {
    vector<string> nextNames;
    int currentMainScene = getNumberCurrentMainScene();

    for (int i = 0; i < number; i++) {
        int index = generalGui.scene - number + i;

        if (index > 0) {
            string sceneFile = timelines.at(index);
            int mainScene = utils.getNumberMainScene(sceneFile);

            if (mainScene == currentMainScene) {
                string name = sceneNames.at(index); //name of the current scene
                nextNames.push_back(name);
            }
        }
    }

    return nextNames;
}

float ofxSceneManagerGui::getRelSceneTimeNormalized() {
    float totSubSceneTime = startTime.nextScene - startTime.curScene;
    float difference = totSubSceneTime - generalGui.relSceneTime;
    float normalizedValue = 1.0 - ofClamp(difference / totSubSceneTime, 0.0, 1.0);

    return normalizedValue;
}

bool ofxSceneManagerGui::loadSceneTimelineGuiXml(int index){
    //load from file
    if(loadSceneBaseXml(sceneTimelineGui, index)){
        
        //update all startTime
        updateStartTime(sceneTimelineGui, index);
        
        return true;
    }
    
    return false;
}

void ofxSceneManagerGui::updateStartTime(SceneTimelineGui &sceneTimelineGui, const int index){
    string fullPath = retriveXmlPath(sceneTimelineGui, index);
    string fullPathNext = retriveXmlPath(sceneTimelineGui, index+1);
    string fullPathPrev = retriveXmlPath(sceneTimelineGui, index-1);
    
    startTime.nextScene = getStartTime(fullPathNext);
    startTime.prevScene = getStartTime(fullPathPrev);
    startTime.curScene = getStartTime(fullPath);
    
    if(utils.isMainScene(fullPath)){
        startTime.curMainScene = startTime.curScene;
    }else{
        //retrive start time of main scene
        string curMainSceneFile = utils.getMainSceneName(timelines.at(index));
        startTime.curMainScene = getStartTime(sceneTimelineGui.getFolder() + "/" + curMainSceneFile);
    }
}

bool ofxSceneManagerGui::loadSceneBaseXml(SceneBaseGui &sceneBaseGui, int index){
    string fullPath = retriveXmlPath(sceneBaseGui, index);
    ofFile xmlSceneFile = ofFile(fullPath);
    
    if(xmlSceneFile.exists()){
        sceneBaseGui.loadFromFile(fullPath);
        sceneBaseGui.setFilename(fullPath);
        return true;
    }
    else{
        ofLogError() << "SceneManager:loadSceneBaseXml: " << fullPath << " not exists.";
    }
    
    return false;
}

bool ofxSceneManagerGui::saveOverrideFileCurrentSceneGui(){
    int index = getGeneralGui().scene;
    if(index >= 0 && index < timelines.size()){
        sceneTimelineGui.saveToFile(sceneTimelineGui.getFilename());
        
        //save other gui
        for(SceneBaseGui &sceneGui : sceneGuiVector){
            sceneGui.saveToFile(sceneGui.getFolder() + "/" + timelines.at(index));
        }
        
        ofLogWarning() << "ofxSceneManagerGui::saveToFileCurrentTimelineParams: saved " << sceneTimelineGui.getFilename();
    
        return true;        //TODO: no controls 
    }
    else{
        ofLogError() << "SceneManager:saveToFileCurrentTimelineParams: " << index << " not exists.";
    }
    
    return false;
}

void ofxSceneManagerGui::deleteFileSceneTimelineGui(SceneTimelineGui &sceneTimelineGui){
    ofFile file(sceneTimelineGui.getFilename(), ofFile::Reference);
    if(file.exists()){        
        for(SceneBaseGui &sceneBaseGui : sceneGuiVector){
            ofFile sceneFile = ofFile(sceneBaseGui.getFolder() + "/" + file.getFileName());
            if(sceneFile.exists()){
				sceneFile.remove();
            }
        }

		file.remove();
    }
}

bool ofxSceneManagerGui::renameSceneTimelineGui(string curFilename, string newFilename){
	//same name not necessary rename
	if (curFilename.compare(newFilename) == 0) {
		return true;
	}

    ofFile sceneFile(sceneTimelineGui.getFolder() + "/" + curFilename);
    if(sceneFile.exists()){
        sceneFile.renameTo(sceneTimelineGui.getFolder() + "/" + newFilename, true, true);

        for(SceneBaseGui &sceneBaseGui : sceneGuiVector){
            ofFile sceneFile = ofFile(sceneBaseGui.getFolder() + "/" + curFilename);
            if(sceneFile.exists()){
                sceneFile.renameTo(sceneBaseGui.getFolder() + "/" + newFilename, true, true);
            }
        }
        
        return true;
    }
    
    return false;
}

bool ofxSceneManagerGui::deleteFileCurrentSceneTimelineGui(){
    int index = getGeneralGui().scene;
    if(index >= 0 && index < timelines.size()){
        string filename = timelines.at(index);
        string path = sceneTimelineGui.getFilename();
        
        if(! utils.isMainScene(path)){
            ofFile file(path);
            if(file.exists()){
                
				//close is necessary to delete after the file
				file.close();

                //delete files
                deleteFileSceneTimelineGui(sceneTimelineGui);
                
                //remove from timeline
                timelines.erase(timelines.begin()+index);
                
                //retrive name base main scene
                string baseSceneName = filename;
                baseSceneName.resize(baseSceneName.size()-6);
                
				//if it is not the last rename
				int count = 1;
				for (string& timeline : timelines) {
					if (timeline.rfind(baseSceneName, 0) == 0) {
						if (!utils.isMainScene(timeline)) {
							string newFilename = baseSceneName + ofToString(count, 2, '0') + ".xml";
							bool success = renameSceneTimelineGui(timeline, newFilename);
							if (success) {
								count++;
							}
						
						}
					}
				}
                
                reloadTimelines(sceneTimelineGui.getFolder());
                
                //force change scene
                generalGui.scene = generalGui.scene - 1;
                forceNextScene();
            }
        }
        else{
            ofLogError() << "ofxSceneManagerGui::deleteCurrentSceneTimelineGui: impossibile cancellare main scene.";
        }
    }
    
    
    return false;
}

void ofxSceneManagerGui::createNewFileSceneTimelineGui(string filename){
    //override current properties
    sceneTimelineGui.autoStart = 1;
    int startTime = generalGui.sceneTime;
    sceneTimelineGui.startTime = startTime;
    
    sceneTimelineGui.saveToFile(sceneTimelineGui.getFolder() + "/" + filename);
    for(SceneBaseGui &sceneBaseGui : sceneGuiVector){
        sceneBaseGui.saveToFile(sceneBaseGui.getFolder() + "/" + filename);
    }
    
    //reload
    loadSceneBaseXml(sceneTimelineGui, generalGui.sceneTime);
}

void ofxSceneManagerGui::createNewFileSceneTimelineGui(){
    string curFileXml = generalGui.sceneFile;
    
    string baseSceneName = curFileXml;
    baseSceneName.resize(baseSceneName.size()-6);
    
    //rename same scene after
    vector<std::reference_wrapper<string>> fileToRename;
    
    for(string &timeline : timelines){
        if (!utils.isMainScene(timeline) && timeline.rfind(baseSceneName, 0) == 0) {
            if(timeline.compare(curFileXml) > 0){
                fileToRename.push_back(timeline);
            }
        }
    }
    
    std::reverse(fileToRename.begin(), fileToRename.end());
    
    for(string &file : fileToRename){
        string newFilename = utils.getShiftedFilename(file, 1);
        renameSceneTimelineGui(file, newFilename);
    }
    
    //save current scene +1
    string newFilename = utils.getShiftedFilename(curFileXml, 1);
    createNewFileSceneTimelineGui(newFilename);
    
    reloadTimelines(sceneTimelineGui.getFolder());
}


#pragma mark Scene Management

void ofxSceneManagerGui::changeScene(int index){
    if(index >= 0 && index < getNumberScenes()){
        bool sceneLoaded = loadSceneTimelineGuiXml(index);
        if(sceneLoaded){
            
            string sceneFile = timelines.at(index);
            string nextMainScene = utils.getNextMainSceneName(sceneFile);
            int nextMainSceneTime = getStartTime(sceneTimelineGui.getFolder() + "/" + nextMainScene);

            //update general gui
            generalGui.scene = index;
            generalGui.sceneFile = timelines.at(index);
            generalGui.sceneTime.setMin(0);
            generalGui.sceneTime.setMax(nextMainSceneTime - startTime.curMainScene);
            generalGui.relSceneTime.setMax(startTime.nextScene - startTime.curScene);

            //TODO: improve callback when load from file
            sceneTimelineGui.setDurationAllTransitionSlider(sceneTimelineGui.transitionTime);
            
            //load other files
            for(SceneBaseGui &sceneBaseGui : sceneGuiVector){
                loadSceneBaseXml(sceneBaseGui, index);
                sceneBaseGui.setDurationAllTransitionSlider(sceneTimelineGui.transitionTime);
            }
            
            updateTime(isPlaying());
            
            //create object for the scene
            Scene scene;
            scene.timelineFilename = sceneFile;
            scene.mainNumber = utils.getNumberMainScene(sceneFile);
            scene.subNumber = utils.getNumberSubScene(sceneFile);
            
            //change other
            ofNotifyEvent(onSceneChangedIndex, index);
            ofNotifyEvent(onSceneChangedString, sceneFile);
            ofNotifyEvent(onSceneChanged, scene);
            
            if(! sceneTimelineGui.autoStart) generalGui.play = false;
        }
        else{
            ofLogError() << "Scene index:" << index << " not loaded";
        }
    }
}

void ofxSceneManagerGui::forceChangeScene(int index){
    if (index >= 0 && index < getNumberScenes()){
        string fullPath = retriveXmlPath(sceneTimelineGui, index);
        int startTime = getStartTime(fullPath);
        
        generalGui.time = startTime;
        
        if(isPlaying())  {
            startShowTimeMillis = ofGetSystemTimeMillis() - startTime;
        }
        else{
            bChangedSceneWhilePause = true;
        }
        
        updateTime(isPlaying());
        
        changeScene(index);
    }
    else{
        ofLogError() << "forceChangeScene index "<< index << " not valid." ;
    }
}

void ofxSceneManagerGui::startShow(){
//    changeScene(generalGui.scene);
    
    startShowTimeMillis = ofGetSystemTimeMillis() - generalGui.time;
    bIsPlaying = true;
    generalGui.play = true;

    bChangedSceneWhilePause = false;
    ofNotifyEvent(onSceneStarted);
}

void ofxSceneManagerGui::pauseShow(){
    bIsPlaying = false;
    pauseTimeMillis = generalGui.time;
    generalGui.play = false;
    
    ofNotifyEvent(onScenePaused);
}

void ofxSceneManagerGui::startAfterPause(){
    if(!bChangedSceneWhilePause){
        startShowTimeMillis = ofGetSystemTimeMillis() - pauseTimeMillis;
    }else{
        startShowTimeMillis = ofGetSystemTimeMillis() - generalGui.time;
    }
    
    bIsPlaying = true;
    bChangedSceneWhilePause = false;
    generalGui.play = true;
    
    updateTime(true);
    ofNotifyEvent(onSceneStarted);
}

void ofxSceneManagerGui::forceBackwardTimeMillis(float millis){
    if(generalGui.time - millis > startTime.curScene){
        if(isPlaying()) {
            startShowTimeMillis += millis;
            generalGui.time = ofGetSystemTimeMillis() - startShowTimeMillis;
        }
       else {
           generalGui.time = generalGui.time - millis;
           bChangedSceneWhilePause = true;
       }
        
        updateTime();

        ofNotifyEvent(onSceneBackwarded);
    }
}

void ofxSceneManagerGui::forceForwardTimeMillis(float millis){
    if(generalGui.time + millis < startTime.nextScene){
        if(isPlaying()) {
            startShowTimeMillis -= millis;
            generalGui.time = ofGetSystemTimeMillis() - startShowTimeMillis;
        }
        else {
            generalGui.time = generalGui.time + millis;
            bChangedSceneWhilePause = true;
        }
    
        updateTime();

        ofNotifyEvent(onSceneForwarded);
    }
}

void ofxSceneManagerGui::forceChangeScene(string timelineFilename){
    int index = 0;
    for(string &s : timelines){
        if(s.compare(timelineFilename) == 0){
            //found
            
            break;
        }
        
        index ++;
    }
    
    forceChangeScene(index);
}

#pragma mark Update

void ofxSceneManagerGui::updateTime(bool updateGuiTime){
    if(updateGuiTime) generalGui.time = ofGetSystemTimeMillis() - startShowTimeMillis;
    generalGui.sceneTime = generalGui.time - startTime.curMainScene;
    generalGui.relSceneTime = generalGui.time - startTime.curScene;
}

void ofxSceneManagerGui::update() {
    //force to stop while is loading
    if (isLoading()) generalGui.play = false;

    if (generalGui.play != bIsPlaying) {
        if (generalGui.play) {
            if (startShowTimeMillis == 0) startShow();
            else {
                startAfterPause();
            }
        }
        else if (!generalGui.play) pauseShow();
    }

    updateTime(isPlaying());

    if (isPlaying()) {

        if (generalGui.time > startTime.nextScene&& generalGui.scene < getNumberScenes() - 1 && (bInfinityTime == false || sceneTimelineGui.infinityTime == false)) {
            clock_t start, end;
            start = clock();
            ios_base::sync_with_stdio(false);

            //next scene
            changeScene(generalGui.scene + 1);

            end = clock();

            // Calculating total time taken by the program.
            double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
            ofLog(OF_LOG_NOTICE) << "ofxSceneManagerGui::time to changeScene is " << time_taken << setprecision(5) << " seconds";
        }
        else {
            sceneTimelineGui.update();

            //update scene base gui
            for (SceneBaseGui& sceneBaseGui : sceneGuiVector)
                sceneBaseGui.update();
        }
    }
    //    else if((! isPlaying() && sceneTimelineGui.autoStart) || generalGui.relSceneTime > 2){
    //        //update scene base gui
    //        for(SceneBaseGui &sceneBaseGui : sceneGuiVector)
    //            sceneBaseGui.update();
    //    }
    else {
        // !! isPlaying()
        if (settings.bJumpToFinalValueTransitionSliderWhilePause) {
            for (SceneBaseGui& sceneBaseGui : sceneGuiVector) {
                sceneBaseGui.jumpToFinalValueTranistionSlider();
            }
        }
        else if (generalGui.relSceneTime > 2) {
            for (SceneBaseGui& sceneBaseGui : sceneGuiVector)
                sceneBaseGui.update();
        }
    }

    int msI = generalGui.relSceneTime;
    std::chrono::milliseconds ms{ msI };
    generalGui.relSceneTimeHr = format_duration(ms);

    int msS = generalGui.time;
    std::chrono::milliseconds ms2{ msS };
    generalGui.timeHr = format_duration(ms2);

    int msR = generalGui.sceneTime;
    std::chrono::milliseconds ms3{ msR };
    generalGui.sceneTimeHr = format_duration(ms3);
}

void ofxSceneManagerGui::draw(){
    generalGui.draw();
    sceneTimelineGui.draw();
}

#pragma mark ofxTransitionSlider Time vs Frame

void ofxSceneManagerGui::setTransitionSliderTimeBased(){
    bTransitionTimeBased = true;
    
    for(SceneBaseGui &sceneBaseGui : sceneGuiVector){
        sceneBaseGui.setTransitionTimeBased(true);
    }
}

void ofxSceneManagerGui::setTransitionSliderFrameBased(int framerateReference){
    bTransitionTimeBased = false;
    
    for(SceneBaseGui &sceneBaseGui : sceneGuiVector){
        sceneBaseGui.setTransitionTimeBased(false, framerateReference);
    }
}

string format_duration( std::chrono::milliseconds ms ) {
    using namespace std::chrono;
    auto secs = duration_cast<seconds>(ms);
    ms -= duration_cast<milliseconds>(secs);
    auto mins = duration_cast<minutes>(secs);
    secs -= duration_cast<seconds>(mins);
    auto hour = duration_cast<hours>(mins);
    mins -= duration_cast<minutes>(hour);

    std::stringstream ss;
    ss << hour.count() << ":" << mins.count() << ":" << secs.count()<< ":" << ms.count();
    return ss.str();
}
