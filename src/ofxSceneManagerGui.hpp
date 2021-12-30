//
//  SceneManager.hpp
//  VanGoghInMe_OF
//
//  Created by Matteo on 20/03/2020.
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include <stdio.h>
#include "ofxTransitionSlider.h"
#include "ofMain.h"
#include "GeneralGui.hpp"
#include "SceneTimelineGui.hpp"
#include "SceneManagerUtils.hpp"
#include "SceneManagerSettings.hpp"
#include "Scene.hpp"

class ofxSceneManagerGui {
    
protected:
    struct StartTimeEnum {
      int prevScene = 0;           //startTime of previuos scene
      int curScene = 0;            //startTime of current scene
      int nextScene = 0;           //startTime of next scene
      int curMainScene = 0;        //startTime of main scene
    } ;
    
public:
    ofxSceneManagerGui();
    ofxSceneManagerGui(SceneManagerSettings &sceneManagerSettings);
    void setup(vector<reference_wrapper<SceneBaseGui>> &sceneGuiVector, ofVec2f &position);
    void startShow();
    void pauseShow();
    void startAfterPause();
    void forceNextScene(){forceChangeScene(generalGui.scene+1);};
    void forcePrevScene(){forceChangeScene(generalGui.scene-1);};
    void forceBackwardTimeMillis(float millis);
    void forceForwardTimeMillis(float millis);
    void forceChangeScene(string timelineFilename);
    
    inline void setSceneFolder(string folder) { sceneFolder = folder; }
    inline string getSceneFolder() {return sceneFolder;}
    
    bool saveOverrideFileCurrentSceneGui();                 //save to file overriding gui params
    bool deleteFileCurrentSceneTimelineGui();                  //delete file of current scene
    void createNewFileSceneTimelineGui();                      //create new scene
    void update();
    void updateTime(bool updateGuiTime = false);
        
    void draw();
    
    int getNumberScenes() {return timelines.size(); };
    int getNumberCurrentMainScene();                    //return the number of current main scece timeline04_00.xml > 4
    int getNumberCurrentSubScene();                     //return the number of current sub scece timeline04_00.xml > 4
    float getRelSceneTimeNormalized();                  //return the normalized value current subscene time
    
    string getCurrentSceneName();
    vector<string> getNextScenesNames(int number);              //return a vector with the name (param inside xml) of the scene
    vector<string> getPreviousScenesNames(int number);          //return a vector with the name (param inside xml) of the scene

    bool isPlaying(){return bIsPlaying ;} ;
    bool isLoading() {return bIsLoading; };

    GeneralGui&            getGeneralGui()          {return generalGui;     };
    SceneTimelineGui&      getSceneTimelineGui()    {return sceneTimelineGui;  };
    SceneManagerSettings&  getSceneManagerSettings(){return settings; };
    vector<string> getTimelines(){return timelines; } ;
    SceneManagerUtils& getUtils(){return utils; };
    
    ofEvent<int>  onSceneChangedIndex;              //event when a scene is changed with index of scene changed. Array is lenght as number of scenes.
    ofEvent<string&>  onSceneChangedString;         //event when a scene is changed with the name of changed scene (ex: timeline10_03.xml
    ofEvent<Scene&>  onSceneChanged;                //event when a scene is changed
    ofEvent<void> onSceneStarted;                   //event when the show start or when start after pause
    ofEvent<void> onScenePaused;                    //event when the show goes to pause
    ofEvent<void> onSceneBackwarded;                //event when the current scene is back
    ofEvent<void> onSceneForwarded;                 //event when the current scene is farward
    
    inline void setEnableInfinityTime(bool enabled) {bInfinityTime = enabled; }
    void setTimeBased();                                        //this method set all transition slider as time based
    void setFrameBased(int framerateReference);                 //this method set all transition slider as frame based
    void setFramerateReference(int framerateReference);         //TODO: there is a bug for the first transition. You need to set this value.
    inline bool isTimeBased(){return bTimeBased;}
    
protected:
    string sceneFolder = "scenes/";             //TODO: add the possibility to choose
    void reloadTimelines(string folder);        //reload timelines files
    
    string retriveXmlPath(SceneBaseGui &sceneBaseGui, int index);

    int getStartTime(string fileXml);                  //return value of param startTime
    bool loadSceneTimelineGuiXml(int index);
    bool loadSceneBaseXml(SceneBaseGui &sceneBaseGui, int index);
    void updateStartTime(SceneTimelineGui &sceneTimelineGui, const int index);

    void forceChangeScene(int index);
    void changeScene(int index);
    void setDurationAllTransitionSlider(int duration);

    bool bIsPlaying = false;
    bool bChangedSceneWhilePause = false;
    
    //FILE MANAGEMENT
    void deleteFileSceneTimelineGui(SceneTimelineGui &sceneTimelineGui);
    bool renameSceneTimelineGui(string currentFilename, string newFilename);
    void createNewFileSceneTimelineGui(string filename);
    
    //timing
    uint64_t currentTimeCodeMillis = 0;         //current time in millis
    uint64_t startShowTimeMillis = 0;           //time when show is started
    uint64_t pauseTimeMillis     = 0;           //store time when show goes to pause

    StartTimeEnum startTime;                    //all time start to manage timing
    bool bIsLoading = false;

    SceneManagerUtils utils;
    
    SceneManagerSettings settings;
    
    //BaseGui needed to have a timeline
    GeneralGui generalGui;
    SceneTimelineGui sceneTimelineGui;
    
    vector<string>timelines;                                 //all .xml file inside scenes/_timeline
    vector<string>sceneNames;                                //value name inside scene
    vector<reference_wrapper<SceneBaseGui>> sceneGuiVector;  //put here all transition slider
    
    bool bInfinityTime = true;
    
    //time/frame based
    bool bTimeBased = true;                                  //if true change scene is time based, if false is frame based
    int framerateReference = 60.0f;                          //fps to calculate the duration of a scene not time based, but frames based
    int durationSceneFrame;                                  //durantion of scene expressed in frame
    int sceneFrameCounter = 0;                               //frameCounter from current scene (relScene)
    int mainSceneFrameCounter = 0;                           //frameCounter from current main scene
    int showFrameCounter = 0;                                //frameCounter from the beginning of the show
    
    void updateFrameCounter(float millis, int framerate);
   };

#endif /* SceneManager_hpp */
