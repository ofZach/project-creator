#include "xcodeProject.h"
#include <iostream>






bool xcodeProject::load(string path){
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(path).c_str());
    
    // todo check result here
    bLoaded = true;
    
    // from this file, find the main.cpp, testApp.cpp and testApp.h in the XML
    // and grab their info (we will use their nodes later when we add files)
    parseForSrc();

	return result.status==pugi::status_ok;
}  


bool xcodeProject::save(string fileName){
    return doc.save_file(ofToDataPath(fileName).c_str());
}  

pugi::xml_node duplicateNodeBelow(pugi::xml_node & node ){
    pugi::xml_node toAdd = node;
    pugi::xml_node added = node.parent().insert_copy_after(toAdd,node);
    return added;
}


bool xcodeProject::findArrayForUUID(string UUID, pugi::xml_node & nodeMe){
    char query[255];
    sprintf(query, "//string[contains(.,'%s')]", UUID.c_str());
    pugi::xpath_node_set uuidSet = doc.select_nodes(query);
    for (pugi::xpath_node_set::const_iterator it = uuidSet.begin(); it != uuidSet.end(); ++it){
        pugi::xpath_node node = *it;
        if (strcmp(node.node().parent().name(), "array") == 0){
            nodeMe = node.node().parent();
            return true;
        } else {
        }
    }
    return false;
}

void xcodeProject::parseForSrc(){
    
   
    srcFiles.clear();
    
    pugi::xpath_node_set source = doc.select_nodes("//string/text()");
    
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
        pugi::xpath_node node = *it;
        if (strcmp(node.node().value(),"sourcecode.cpp.cpp") == 0 || 
            strcmp(node.node().value(),"sourcecode.c.h") == 0 ){
            
            xcodeSrcFile file;
            
            string UUID = string(node.node().parent().parent().previous_sibling().first_child().value());
            file.UUID = UUID;
            
            // 4 = file name 
            pugi::xpath_node_set set = node.node().parent().parent().select_nodes("string[4]");
            file.fileName = set.begin()->node().first_child().value();
            
            // 5 = file path
            set = node.node().parent().parent().select_nodes("string[5]");
            file.filePath = set.begin()->node().first_child().value();
           
            // grab the parent node. 
            file.srcNode = node.node().parent().parent();
            srcFiles.push_back(file);
            
        }
    }
    

    for (int i = 0; i < (int)srcFiles.size(); i++){
        // one is an "array", the second 
        
        char query[255];
        sprintf(query,"//string[contains(.,'%s')]",  srcFiles[i].UUID.c_str());
        
        pugi::xpath_node_set uuidSet = doc.select_nodes(query);
        for (pugi::xpath_node_set::const_iterator it = uuidSet.begin(); it != uuidSet.end(); ++it){
            pugi::xpath_node node = *it;
            if (strcmp(node.node().parent().name(), "array") == 0){
                srcFiles[i].srcArrayNode = node.node().parent();
            } else {
                
                string buildRefUUID = string(node.node().parent().previous_sibling().first_child().value());
                srcFiles[i].buildRefUUID = buildRefUUID;
                srcFiles[i].buildRefNode = node.node().parent();
                pugi::xml_node buildRefArray;
                findArrayForUUID(buildRefUUID, buildRefArray);
                srcFiles[i].buildRefArrayNode = buildRefArray;
            }
        }
    }
    
    /*
    // now, let's output everything, to make sure it's all good. 
    for (int i = 0; i < srcFiles.size(); i++){
        printf("----------------------------------------- \n");
        cout << srcFiles[i].fileName << " " << srcFiles[i].filePath << endl;
        cout << srcFiles[i].UUID << endl;
        printf("-- src node -- \n");
        srcFiles[i].srcNode.print(std::cout);
        printf("-- src array -- \n");
        srcFiles[i].srcArrayNode.print(std::cout);
        printf("--------- ref info \n");
        cout << srcFiles[i].buildRefUUID << endl;
        printf("-- buildRef node -- \n");
        srcFiles[i].buildRefNode.print(std::cout);
        printf("-- buildRef array -- \n");
        srcFiles[i].buildRefArrayNode.print(std::cout);
    }
    */

}


// this is all UUID related: 
// generate new UUIDS for each file that are random enough. 

int unixTimeAtLaunch;
xcodeProject::xcodeProject(){
    unixTimeAtLaunch = ofGetUnixTime();
}

int counter = 0;
string generateUUID(){
    ofSeedRandom( unixTimeAtLaunch + counter++);
    
    char UPPER_DIGITS[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F',
    };
    string result;
    for (int i= 0; i < 24; i++){
        result += UPPER_DIGITS[(int)ofRandom(0,10000000) % 16];
    }
    return result;
}



void xcodeProject::addSrc(string srcFile, string folder){
    
    
    // todo, there's some duplicate code here...
    
    
    if (ofIsStringInString(srcFile, ".h") || 
        ofIsStringInString(srcFile, ".hpp")  ){
        
        // assume that the 3rd item in the src files is "testApp.h":
        xcodeSrcFile testAppH = srcFiles[2];
        string UUID = generateUUID();
        //printf("addind src %s, UUID %s \n", srcFile.c_str(), UUID.c_str());
        
        // from the srcFile, get a path and fileName out of it.  (path here includes fileName)
        string srcFolder, srcName;
        if (ofIsStringInString(srcFile, "/")){
            size_t found = srcFile.find_last_of("/");
            srcFolder    = srcFile; //srcFile.substr(0,found);
            srcName      = srcFile.substr(found+1);
        } else {
            srcFolder    = srcFile;
            srcName      = srcFile;
        }
        
        
        // add a node with this info
        // string[4] = fileName, string[5] = filePath
        pugi::xml_node added = duplicateNodeBelow(testAppH.srcNode);
        pugi::xpath_node_set set = added.select_nodes("string[4]");
        set.begin()->node().first_child().set_value(srcName.c_str());
        set = added.select_nodes("string[5]");
        set.begin()->node().first_child().set_value(srcFolder.c_str());
        
        // add a key with a generated UUID
        pugi::xml_node key = testAppH.srcNode.previous_sibling();
        pugi::xml_node addedKey = testAppH.srcNode.parent().insert_copy_before(key,added);
        addedKey.first_child().set_value(UUID.c_str());
        
        // add it to the SOURCE array: 
        pugi::xml_node arrayAdd = testAppH.srcArrayNode.append_child("string");
        arrayAdd.append_child(pugi::node_pcdata).set_value(UUID.c_str());
        
    } else {
        
        // assume that the 1st item in the src files is "main.cpp" -- this might not be the case
        xcodeSrcFile testAppC = srcFiles[0];
        string UUID = generateUUID();
        //printf("addind src %s, UUID %s \n", srcFile.c_str(), UUID.c_str());
        
        string srcFolder, srcName;
        if (ofIsStringInString(srcFile, "/")){
            size_t found = srcFile.find_last_of("/");
            srcFolder    = srcFile; //srcFile.substr(0,found);
            srcName      = srcFile.substr(found+1);
        } else {
            srcFolder    = srcFile;
            srcName      = srcFile;
        }
        
        // add a node with this info
        pugi::xml_node added = duplicateNodeBelow(testAppC.srcNode);
        pugi::xpath_node_set set = added.select_nodes("string[4]");
        added.select_nodes("string[4]").begin()->node().first_child().set_value(srcName.c_str());
        added.select_nodes("string[5]").begin()->node().first_child().set_value(srcFolder.c_str());
        
        // add a key with a generated UUID
        pugi::xml_node key = testAppC.srcNode.previous_sibling();
        pugi::xml_node addedKey = testAppC.srcNode.parent().insert_copy_before(key,added);
        addedKey.first_child().set_value(UUID.c_str());
        
        // get this UUID into source array
        pugi::xml_node arrayAdd = testAppC.srcArrayNode.append_child("string");
        arrayAdd.append_child(pugi::node_pcdata).set_value(UUID.c_str());
        
        // copy the build ref node:
        string buildUUID = generateUUID();
        
        pugi::xml_node buildRefAdded = duplicateNodeBelow(testAppC.buildRefNode);
        //buildRefAdded.print(std::cout);
        buildRefAdded.select_nodes("string[1]").begin()->node().first_child().set_value(UUID.c_str());
        key = testAppC.buildRefNode.previous_sibling();
        addedKey = testAppC.buildRefNode.parent().insert_copy_before(key,buildRefAdded);
        addedKey.first_child().set_value(buildUUID.c_str());
        
        // now get this build ref in the build array
        pugi::xml_node buildArrayAdd = testAppC.buildRefArrayNode.append_child("string");
        buildArrayAdd.append_child(pugi::node_pcdata).set_value(buildUUID.c_str());
        
        
    }
} 

void xcodeProject::addInclude(string includeName){
    
}  
void xcodeProject::addLibrary(string libraryName){
        
}

void xcodeProject::addAddon(ofAddon & addon){

}

string xcodeProject::getName(){

}

string xcodeProject::getPath(){

}
