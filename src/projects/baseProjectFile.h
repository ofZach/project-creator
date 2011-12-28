

#pragma once


#include "ofMain.h"
#include "pugixml.hpp"

class baseProjectFile {
    
    public: 
    
    baseProjectFile(){
        bLoaded = false;
    };
    virtual ~baseProjectFile(){};
    
    virtual void loadFile(string fileName) = 0;
    virtual void saveFile(string fileName) = 0;

    virtual void addSrc(string srcFile) = 0;
    virtual void addInclude(string includeName) = 0;
    virtual void addLibrary(string libraryName) = 0;

    //private: 

    bool doesTagAndAttributeExist(pugi::xml_document & doc, string tag, string attribute, string newValue);
    void appendValue(pugi::xml_document & doc, string tag, string attribute, string newValue);

    pugi::xml_document doc;
    bool bLoaded;
    
};


