#pragma once


#include "ofMain.h"
#include "pugixml.hpp"


class ofAddon {
    
public: 
    
    
    string addonPath;
    
    
    map < string, string > filesToFolders;      //the addons has had, for each file, 
                                                //sometimes a listing of what folder to put it in, such as "addons/ofxOsc/src"
    
    // src, libs, include paths
    vector < string > srcFilesToAdd;
    vector < string > libsToAdd;
    vector < string > includePathsToAdd;
    
    
    void printToConsole(){
        
        if (addonPath.size() > 0)
        printf("------------- addon (%s) ---------- \n", addonPath.c_str());
        //printf("hi \n");
        //return;
        for (int i = 0; i < srcFilesToAdd.size(); i++)
            printf("--- src  : %s \n", srcFilesToAdd[i].c_str());
        for (int i = 0; i < includePathsToAdd.size(); i++)
            printf("--- inc  : %s \n", includePathsToAdd[i].c_str());
        for (int i = 0; i < libsToAdd.size(); i++)
            printf("--- link : %s \n", libsToAdd[i].c_str());
        
        printf("----------------------------------- \n");
        
    }
    
    
};


class addonsParser {
    
public: 
    
    
    
    void loadAddons();
    
    vector < ofAddon > addonsFromXML;
    vector < ofAddon > addonsFromRecursive;
    
    
};

