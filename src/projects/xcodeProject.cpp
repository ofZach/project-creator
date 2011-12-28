


#include "xcodeProject.h"


#include <iostream>

//const char* node_types[] =
//{
//    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
//};
//
////[code_traverse_walker_impl
//struct simple_walker: pugi::xml_tree_walker
//{
//    virtual bool for_each(pugi::xml_node& node)
//    {
//        for (int i = 0; i < depth(); ++i) std::cout << "  "; // indentation
//        
//        std::cout << node_types[node.type()] << ": name='" << node.name() << "', value='" << node.value() << "'\n";
//        
//        return true; // continue traversal
//    }
//};
////]




bool xcodeProject::load(string path){
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(path).c_str());
    // todo check result here
    bLoaded = true;
    parseForSrc();

	return result.status==pugi::status_ok;
}  





bool xcodeProject::save(string fileName){
    return doc.save_file(ofToDataPath(fileName).c_str());

}  


bool xcodeProject::create(string fileName){
	return true;
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
            
            
            pugi::xpath_node_set set = node.node().parent().parent().select_nodes("string[4]");
            file.fileName = set.begin()->node().first_child().value();
            //cout << UUID << " " << set.begin()->node().first_child().value() << endl;
            
            set = node.node().parent().parent().select_nodes("string[5]");
            file.filePath = set.begin()->node().first_child().value();
            //cout << "path : " << " " << set.begin()->node().first_child().value() << endl;
            
            //file.nodeHash = node.node().parent().parent().hash_value();
            //cout << "this nodes hash: " << node.node().parent().parent().hash_value() << endl; 
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
                // this is the "buildRef"
                //cout <<  node.node().parent().name() <<endl;
                //cout << string(node.node().parent().previous_sibling().first_child().value()) << endl;
                string buildRefUUID = string(node.node().parent().previous_sibling().first_child().value());
                
                srcFiles[i].buildRefUUID = buildRefUUID;
                srcFiles[i].buildRefNode = node.node().parent();
                
                pugi::xml_node buildRefArray;
                findArrayForUUID(buildRefUUID, buildRefArray);
                //simple_walker walker;
                //cout << buildRefArray.name() << endl;
                //buildRefArray.traverse(walker);
                //buildRefArray.print(std::cout);
                
                srcFiles[i].buildRefArrayNode = buildRefArray;
                // this array is what gets built.
                // let's get the array associated with this build ref UUID
                
            }
        }
    }
    
    /*
    // now, let's output everything!
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

string generateUUID(){
    ofSeedRandom(ofGetUnixTime());
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
    
    
   
    if (ofIsStringInString(srcFile, ".h") || 
        ofIsStringInString(srcFile, ".hpp")  ){
        
        // assume that the 3rd item in the src files is "testApp.h" -- this might not be the case
        xcodeSrcFile testAppH = srcFiles[2];
        
        string UUID = generateUUID();
    
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
        pugi::xml_node added = testAppH.srcNode.parent().insert_copy_after(testAppH.srcNode,testAppH.srcNode);
        pugi::xpath_node_set set = added.select_nodes("string[4]");
        set.begin()->node().first_child().set_value(srcName.c_str());
        set = added.select_nodes("string[5]");
        set.begin()->node().first_child().set_value(srcFolder.c_str());
        
        // add a key with a generated UUID
        pugi::xml_node key = testAppH.srcNode.previous_sibling();
        pugi::xml_node addedKey = testAppH.srcNode.parent().insert_copy_before(key,added);
        addedKey.first_child().set_value(UUID.c_str());
        
        // add it to the SOURCE array: 
        
        
    } else {
        
    }
    
    // so we want to throw into the array, 
    // and duplicate this fileRef
    // the fileRef also gets added to a build step if this is a .cpp file. 
    
     /*
     // source files appear here: 
     <key>E4B69E1C0A3A1BDC003C02F2</key>
     <dict>
     <key>children</key>
     <array>
     <string>E4B69E1D0A3A1BDC003C02F2</string>
     <string>E4B69E1E0A3A1BDC003C02F2</string>
     <string>E4B69E1F0A3A1BDC003C02F2</string>
     </array>
     <key>isa</key>
     <string>PBXGroup</string>
     <key>path</key>
     <string>src</string>
     <key>sourceTree</key>
     <string>SOURCE_ROOT</string>
     </dict>
     
     and here: 
     
     
     <key>E4B69E200A3A1BDC003C02F2</key>
     <dict>
     <key>fileRef</key>
     <string>E4B69E1D0A3A1BDC003C02F2</string>
     <key>isa</key>
     <string>PBXBuildFile</string>
     </dict>
     
     if they are compiled
     
     compile options appear here:
     
     <key>E4B69B580A3A1756003C02F2</key>
     <dict>
     <key>buildActionMask</key>
     <string>2147483647</string>
     <key>files</key>
     <array>
     <string>E4B69E200A3A1BDC003C02F2</string>
     <string>E4B69E210A3A1BDC003C02F2</string>
     </array>
     <key>isa</key>
     <string>PBXSourcesBuildPhase</string>
     <key>runOnlyForDeploymentPostprocessing</key>
     <string>0</string>
     </dict>
     
     craziness...
     */
    
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
