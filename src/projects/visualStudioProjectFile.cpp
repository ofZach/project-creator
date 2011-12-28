


#include "visualStudioProjectFile.h"


void fixSlashOrder(string & toFix){
    std::replace(toFix.begin(), toFix.end(),'/', '\\');
}


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
    
    fixSlashOrder(srcFile);
    
    if (ofIsStringInString(srcFile, ".h") || ofIsStringInString(srcFile, ".hpp")){
        appendValue(doc, "ClInclude", "Include", srcFile);
    } else {
        appendValue(doc, "ClCompile", "Include", srcFile);
    }
    
} 

void visualStudioProjectFile::addInclude(string includeName){
   
    
    fixSlashOrder(includeName);
    
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
            string includesNew = unsplitString(strings, ";");
            node.node().first_child().set_value(includesNew.c_str());
        }
       
    }
    //appendValue(doc, "Add", "directory", includeName);
}  
void visualStudioProjectFile::addLibrary(string libraryName){

    
    fixSlashOrder(libraryName);
    
    // ok first, split path and library name.  
    size_t found = libraryName.find_last_of("/");
    string libFolder = libraryName.substr(0,found);
    string libName = libraryName.substr(found+1);
    
    // do the path, then the library
    
    // paths for libraries
    pugi::xpath_node_set source = doc.select_nodes("//Link/AdditionalLibraryDirectories");
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
        pugi::xpath_node node = *it;
        string includes = node.node().first_child().value();
        vector < string > strings = ofSplitString(includes, ";");
        bool bAdd = true;
        for (int i = 0; i < strings.size(); i++){
            if (strings[i].compare(libFolder) == 0){
                bAdd = false;
            }
        }
        if (bAdd == true){
            strings.push_back(libFolder);
            string libPathsNew = unsplitString(strings, ";");
            cout << libPathsNew << endl;
            node.node().first_child().set_value(libPathsNew.c_str());
        }
    }
    
    // libs
    source = doc.select_nodes("//Link/AdditionalDependencies");
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
        pugi::xpath_node node = *it;
        string includes = node.node().first_child().value();
        vector < string > strings = ofSplitString(includes, ";");
        bool bAdd = true;
        for (int i = 0; i < strings.size(); i++){
            if (strings[i].compare(libName) == 0){
                bAdd = false;
            }
        }
        if (bAdd == true){
            strings.push_back(libName);
            string libsNew = unsplitString(strings, ";");
            cout << libsNew << endl;
            node.node().first_child().set_value(libsNew.c_str());
        }
       
    }


}
