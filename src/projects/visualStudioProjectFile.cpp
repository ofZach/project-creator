


#include "visualStudioProjectFile.h"


string unsplitString (vector < string > strings, string deliminator ){
    string result;
    for (int i = 0; i < strings.size(); i++){
        if (i != 0) result += deliminator;
        result += strings[i];
    }
    return result;
}


void visualStudioProjectFile::loadFile(string fileName){
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(fileName).c_str());
    // todo check result here
    bLoaded = true;
}  


void visualStudioProjectFile::saveFile(string fileName){
    doc.save_file(ofToDataPath(fileName).c_str());
}  

void visualStudioProjectFile::addSrc(string srcFile){
    
    
    
    //appendValue(doc, "Unit", "filename", srcFile);
} 
void visualStudioProjectFile::addInclude(string includeName){
   
    pugi::xpath_node_set source = doc.select_nodes("//ClCompile/AdditionalIncludeDirectories");
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
        //printf("here ?? \n");
        pugi::xpath_node node = *it;
        string includes = node.node().first_child().value();
        vector < string > strings = ofSplitString(includes, ";");
        bool bAdd = true;
        for (int i = 0; i < strings.size(); i++){
            if (strings[i].compare(includeName) == 0){
                bAdd = false;
            }
        }
        if (bAdd == true){
            strings.push_back(includeName);
        }
        string includesNew = unsplitString(strings, ";");
        node.node().first_child().set_value(includesNew.c_str());
    }
    //appendValue(doc, "Add", "directory", includeName);
}  
void visualStudioProjectFile::addLibrary(string libraryName){
    //appendValue(doc, "Add", "library", libraryName);
}
