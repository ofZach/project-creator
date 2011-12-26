

#pragma once

#include "baseProjectFile.h"

class codeblocksProjectFile : public baseProjectFile {
    
public: 
    void loadFile(string fileName);
    void saveFile(string fileName);
    void addSrc(string srcFile);
    void addInclude(string includeName);
    void addLibrary(string libraryName);

};
