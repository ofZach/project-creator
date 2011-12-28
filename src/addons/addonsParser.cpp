
#include "addonsParser.h"



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
        addon.srcFilesToAdd.push_back(node.node().child_value());
    }
    
    
    add = doc.select_nodes("//include/path");
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        addon.includePathsToAdd.push_back(node.node().child_value());
        //std::cout << "include: " << node.node().child_value() << endl;
    }
    
    
    add = doc.select_nodes("//link/lib[@compiler='codeblocks']");
    // this has to be smarter I guess...
    for (pugi::xpath_node_set::const_iterator it = add.begin(); it != add.end(); ++it){
        pugi::xpath_node node = *it;
        addon.libsToAdd.push_back(node.node().child_value());
        //std::cout << "link: " << node.node().child_value() << endl;
    }
    
    
}

void addonsParser::loadAddons(){
    
    
    ofDirectory addonsDir;
    addonsDir.listDir("../../../../../addons");
    
    for (int i = 0; i < addonsDir.size(); i++){
        
        ofDirectory addonsLister;
        
        if (!ofIsStringInString(addonsDir.getPath(i), ".h")){       // avoid ofAddons.h 
            addonsLister.listDir(addonsDir.getPath(i));
            if (addonsLister.isDirectory()){
                
                // 1st, try doing a manual parse of the addon: 
                ofAddon manualAddon;
                manualAddon.addonPath = addonsDir.getPath(i);
                parseAddonManually(addonsDir.getFile(i).getPocoFile().path(), manualAddon);
                addonsFromRecursive.push_back(manualAddon);
                
                // 2nd, look for an install.xml and try grabbing that. 
                for (int j = 0; j < addonsLister.size(); j++){
                    if (ofIsStringInString(addonsLister.getPath(j), "install.xml")){
                        ofAddon xmlAddon;
                        xmlAddon.addonPath = addonsDir.getPath(i);
                        turnAddonXMLIntoAddon(addonsLister.getPath(j), xmlAddon);
                        addonsFromXML.push_back(xmlAddon);
                    }
                }
            }
        }
    } 
    
    // print out manual then XML based addons 
    
    printf("================= manual parsed =================  \n");
    for (int i = 0; i < addonsFromRecursive.size(); i++){
        addonsFromRecursive[i].printToConsole();
    }
    
    printf("================= install.xml parsed =================  \n");
    for (int i = 0; i < addonsFromXML.size(); i++){
        addonsFromXML[i].printToConsole();
    }
}

