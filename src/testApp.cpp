#include "testApp.h"
#include "pugixml.hpp"


#include <Poco/DirectoryIterator.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>
#include <iostream>
#include <string>

using namespace Poco;
using namespace std;




void getFilesRecursively(const string & path, vector < string > & fileNames){
   
        DirectoryIterator end;    
        for (DirectoryIterator it(path); it != end; ++it){
            if (!it->isDirectory()){
                fileNames.push_back(it->path());
            }
            if (it->isDirectory()){
                getFilesRecursively(it->path(), fileNames);
            }
        }
    
}


void getLibsRecursively(const string & path, vector < string > & libFiles, vector < string > & libLibs ){
       DirectoryIterator end;    
        for (DirectoryIterator it(path); it != end; ++it){
            if (!it->isDirectory()){
                
                if (ofIsStringInString(it->path(), ".a") ||
                    ofIsStringInString(it->path(), ".lib") ||
                    ofIsStringInString(it->path(), ".dylib")){
                    libLibs.push_back(it->path());
                } else {
                    libFiles.push_back(it->path());
                }
            }
            
            if (it->isDirectory()){
                getLibsRecursively(it->path(), libFiles, libLibs);
            }
        }
    
}





void parseAddonManually(string path,ofAddon & addon){
    
    
    string filePath = path + "/src";
    getFilesRecursively(filePath, addon.srcFilesToAdd);
    
    string libsPath = path + "/libs";
    vector < string > libFiles;
    vector < string > libLibs;
    if (ofDirectory::doesDirectoryExist(libsPath)){
        getLibsRecursively(libsPath, libFiles, addon.libsToAdd);
    }

    // I need to add libFiles to srcFiles
    for (int i = 0; i < libFiles.size(); i++){
        addon.srcFilesToAdd.push_back(libFiles[i]);
    }

    // pull off the ../../../
    // this +12 is **very** mac specific as poco doesn't know about ofToDataPath, etc. 
    
    for (int i = 0; i < addon.srcFilesToAdd.size(); i++){
        addon.srcFilesToAdd[i].erase (addon.srcFilesToAdd[i].begin(), addon.srcFilesToAdd[i].begin()+12);
    }
    
    for (int i = 0; i < addon.libsToAdd.size(); i++){
        addon.libsToAdd[i].erase (addon.libsToAdd[i].begin(), addon.libsToAdd[i].begin()+12);
    }
    
    // get a unique list of the paths that are needed for the includes. 
    
    list < string > paths;
    for (int i = 0; i < addon.srcFilesToAdd.size(); i++){
        size_t found;
        found = addon.srcFilesToAdd[i].find_last_of("/");
        paths.push_back(addon.srcFilesToAdd[i].substr(0,found));
    }
    paths.sort(); 
    paths.unique();
    for (list<string>::iterator it=paths.begin(); it!=paths.end(); ++it)
        addon.includePathsToAdd.push_back(*it);
    
    
    /*
    printf("------------------------- manual (%s) \n", path.c_str());
    for (int i = 0; i < addon.srcFilesToAdd.size(); i++){
        cout << "src --- " << addon.srcFilesToAdd[i] <<endl;
    }
    for (int i = 0; i < addon.libsToAdd.size(); i++){
        cout << "libs --- " <<  addon.libsToAdd[i] <<endl;
    }
    for (int i = 0; i < addon.includePathsToAdd.size(); i++){
        cout << "include paths ---- " << addon.includePathsToAdd[i] << endl;
    }
    */  
    
    
    
}


void turnAddonXMLIntoAddon(string installXmlName, ofAddon & addon){
    
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(installXmlName).c_str());
    
    // this is src to add: 
    pugi::xpath_node_set add = doc.select_nodes("//add/src/folder/file");
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "folder name "  << node.node().parent().attribute("name").value() << " : ";
        //std::cout << "src: " << node.node().child_value() << endl;
    }
    
    
    add = doc.select_nodes("//include/path");
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "include: " << node.node().child_value() << endl;
    }

    
    add = doc.select_nodes("//link/lib[@compiler='codeblocks']");
    // this has to be smarter I guess...
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        //std::cout << "link: " << node.node().child_value() << endl;
    }
    
    
}


bool doesTagAndAttributeExist(pugi::xml_document & doc, string tag, string attribute, string newValue){
    char xpathExpressionExists[1024];
    sprintf(xpathExpressionExists, "//%s[@%s='%s']", tag.c_str(), attribute.c_str(), newValue.c_str());
    cout <<xpathExpressionExists <<endl;
    pugi::xpath_node_set set = doc.select_nodes(xpathExpressionExists);
    if (set.size() != 0){
        return true;
    } else {
        return false;
    }
}

void appendValue(pugi::xml_document & doc, string tag, string attribute, string newValue){
    
    if (!doesTagAndAttributeExist(doc, tag, attribute, newValue)){
        // otherwise, add it please: 
        char xpathExpression[1024];
        sprintf(xpathExpression, "//%s[@%s]", tag.c_str(), attribute.c_str());
        pugi::xpath_node_set add = doc.select_nodes(xpathExpression);
        pugi::xml_node node = add[add.size()-1].node();
        pugi::xml_node nodeAdded = node.parent().append_copy(node);
        nodeAdded.attribute(attribute.c_str()).set_value(newValue.c_str());
    }
}


void addSrc(pugi::xml_document & doc, string srcName){
    appendValue(doc, "Unit", "filename", srcName);
}

void addInclude(pugi::xml_document & doc, string includeName){
    appendValue(doc, "Add", "directory", includeName);
}

void addLibrary(pugi::xml_document & doc, string libraryName){    
    appendValue(doc, "Add", "library", libraryName);
}


//--------------------------------------------------------------
void testApp::setup(){
//codeblocks_windows/emptyExample.cbp
    
    
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(ofToDataPath("codeblocks_windows/emptyExample.cbp").c_str());

    
    addSrc(doc, "src/testApp.cpp");
    addSrc(doc, "testApp3.cpp");
    addSrc(doc, "testApp4.cpp");
    addInclude(doc, "../../../addons/test.cpp");
    addInclude(doc, "../../../addons/test.h");
    addLibrary(doc, "../../../addons/blah.lib");
    
    
    ofDirectory addonsDir;
    addonsDir.listDir("../../../../../addons");
    for (int i = 0; i < addonsDir.size(); i++){
        //cout << addonsDir.getPath(i) << endl;
        //cout << addonsDir.getName(i) << endl;
        ofDirectory addonsLister;
        if (!ofIsStringInString(addonsDir.getPath(i), ".h")){       // avoid ofAddons.h 
            addonsLister.listDir(addonsDir.getPath(i));
            if (addonsLister.isDirectory()){
                
                
                ofAddon temp2;
                parseAddonManually(addonsDir.getFile(i).getPocoFile().path(), temp2);
                
                
                //cout << addonsLister.size() << endl;
                for (int j = 0; j < addonsLister.size(); j++){
                    if (ofIsStringInString(addonsLister.getPath(j), "install.xml")){
                        cout << addonsLister.getPath(j) << endl;
                        
                        ofAddon temp;
                        turnAddonXMLIntoAddon(addonsLister.getPath(j), temp);
                    }
                }
            }
        }
        
    } 
    
    
    
    
    doc.save_file(ofToDataPath("codeblocks_windows/test.cbp").c_str());
}




//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


// use XPATH expressions

/*
 cout << "--------------- source: " << endl;
 pugi::xpath_node_set source = doc.select_nodes("//Unit");
 for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
 pugi::xpath_node node = *it;
 std::cout << " " << node.node().attribute("filename").value() << endl;
 }
 
 cout << "--------------- includes: " << endl;
 pugi::xpath_node_set add = doc.select_nodes("//Add[@directory]");
 for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
 pugi::xpath_node node = *it;
 std::cout << "directory: " << node.node().attribute("directory").value() << endl;
 }
 
 cout << "--------------- linking: " << endl;
 add = doc.select_nodes("//Add[@library]");
 for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
 pugi::xpath_node node = *it;
 std::cout << "library: " << node.node().attribute("library").value() << endl;
 }
 */

