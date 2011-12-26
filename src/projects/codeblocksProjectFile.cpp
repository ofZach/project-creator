

#include "codeblocksProjectFile.h"


void codeblocksProjectFile::loadFile(string fileName){
    
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(fileName).c_str());
    
    // todo check result here
    bLoaded = true;
    
}  


void codeblocksProjectFile::saveFile(string fileName){
    
    doc.save_file(ofToDataPath(fileName).c_str());

}  

void codeblocksProjectFile::addSrc(string srcFile){
    appendValue(doc, "Unit", "filename", srcFile);
} 
void codeblocksProjectFile::addInclude(string includeName){
    appendValue(doc, "Add", "directory", includeName);
}  
void codeblocksProjectFile::addLibrary(string libraryName){
   appendValue(doc, "Add", "library", libraryName);
}
