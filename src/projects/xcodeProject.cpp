#include "xcodeProject.h"
#include <iostream>
#include "Utils.h"

#include "Poco/HMACEngine.h"
#include "Poco/MD5Engine.h"

using Poco::DigestEngine;
using Poco::HMACEngine;
using Poco::MD5Engine;


#define STRINGIFY(A)  #A


    #define STRINGIFY(A)  #A

    const char shader[] = 
    STRINGIFY(

    multiline code goes here

    );


/*
//http://code.google.com/p/pugixml/source/browse/trunk/docs/samples/modify_add.cpp
// this is how to add a tag with a value. 
pugi::xml_node descr = node.append_child("description");
descr.append_child(pugi::node_pcdata).set_value("Simple node");
 */


//  this gets the names of all PBXGroup "group" tags....
//  not the ideal way to deal with folder tho. 
//  string[contains(.,'&lt;group')]/parent::node()//string[contains(.,'PBXGroup')]/parent::node()//key[contains(.,'name')]/following-sibling::node()[1]




// notes for include / linking
// it's in the dict here. (there are 2, release and debug) 

//  <dict>
//  <key>baseConfigurationReference</key>


// XPATH:   //key[contains(.,'baseConfigurationReference')]/parent::node()


// it would look like: 

//    <key>HEADER_SEARCH_PATHS</key>
//    <array>
//    <string>../../../includeTest.h</string>
//    <string>$(OF_CORE_HEADERS)</string>
//    </array>

//    <key>LIBRARY_SEARCH_PATHS</key>
//    <array>
//    <string>../../../librarySearchPath</string>
//    </array>

//    <key>OTHER_CPLUSPLUSFLAGS</key>
//    <array>
//    <string>-D__MACOSX_CORE__</string>
//    <string>-lpthread</string>
//    </array>

//    <key>OTHER_LDFLAGS</key>
//    <array>
//    <string>testLib.a</string>
//    <string>$(OF_CORE_LIBS)</string>
//    </array>

// we need to add

// HEADER_SEARCH_PATHS
// LIBRARY_SEARCH_PATHS
// OTHER_LDFLAGS

// if they don't exist, 
// because with the config files, this stuff is kind of missing. 
// I would guess anywhere in that dict is ok, order seems not to matter. 

// check if they exist: 
//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'HEADER_SEARCH_PATHS')]
//if they exist, get the array
//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'HEADER_SEARCH_PATHS')]//array


//if it exists, it looks like this to get to the array: 

//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'OTHER_CPLUSPLUSFLAGS')]/following-sibling::node()

//          returns

//        <array>
//        <string>-D__MACOSX_CORE__</string>
//        <string>-lpthread</string>
//        </array>
//        <array>
//        <string>-D__MACOSX_CORE__</string>
//        <string>-lpthread</string>
//        </array>



const char PBXGroup[] = 
STRINGIFY(

    <key>GROUPUUID</key>
    <dict>
        <key>children</key>
        <array>
        </array>
        <key>isa</key>
        <string>PBXGroup</string>
        <key>path</key>
        <string>src</string>
        <key>sourceTree</key>
        <string>&lt;group&gt;</string>      // <group> or SOURCE_ROOT, etc
    </dict>
          
);

const char PBXFileReference[] = 
STRINGIFY(

        <key>FILEUUID</key>
        <dict>
            <key>explicitFileType</key>
            <string>FILETYPE</string>
            <key>fileEncoding</key>
            <string>30</string>
            <key>isa</key>
            <string>PBXFileReference</string>
            <key>name</key>
            <string>FILENAME</string>
            <key>path</key>
            <string>FILEPATH</string>
            <key>sourceTree</key>
            <string>SOURCE_ROOT</string>
        </dict>

);

const char PBXBuildFile[] = 
STRINGIFY(
          
          <key>BUILDUUID</key>
          <dict>
          <key>fileRef</key>
          <string>FILEUUID</string>
          <key>isa</key>
          <string>PBXBuildFile</string>
          </dict>
          
);






bool xcodeProject::load(string path){
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(path).c_str());
    
    // todo check result here
    bLoaded = true;
    
    // from this file, find the main.cpp, testApp.cpp and testApp.h in the XML
    // and grab their info (we will use their nodes later when we add files)
    //parseForSrc();
    
    //makeGroupNode("blah", "blasdhsadf");
    //std::exit(0);
    
	return result.status==pugi::status_ok;
}  


bool xcodeProject::save(string fileName){
    return doc.save_file(ofToDataPath(fileName).c_str());
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

//void recurivePathSearch(path * pathToSearch, string name)


pugi::xml_node findOrMakeFolderSet(vector < string > folders){
    
    pugi::xml_node folderSet;
    if (folders.size() == 0) return folderSet;
    
    if (folders[0] == "src"){
        //key[contains(.,"E4B69E1C0A3A1BDC003C02F2")]/following-sibling::node()[1]//array/string/text()
    } else if (folders[0] == "addons"){
        //key[contains(.,"BB4B014C10F69532006C3DED")]/following-sibling::node()[1]//array/string/text()
    } else {
        // I'm not sure the best way to proceed;
    
    }
    
}


//  this is for SRC
//  key[contains(.,"E4B69E1C0A3A1BDC003C02F2")]/following-sibling::node()[1]
//  this is for addons
//  key[contains(.,"BB4B014C10F69532006C3DED")]/following-sibling::node()[1]

//  key[contains(.,"E4B69E1C0A3A1BDC003C02F2")]/following-sibling::node()[1]//array/string/text()
//  E4B69E1F0A3A1BDC003C02F2
// 
//  gets you all keys for pbxgroups
//  string[contains(.,'PBXGroup')]/parent::node()/preceding-sibling::node()[1]
//  checks, does a group exist, for this key?
//  string[contains(.,'PBXGroup')]/parent::node()/preceding-sibling::node()[1][contains(.,'BB4B014C10F69532006C3DED')]


// does group exist
// add group to group
// add fileToGroup


//bool xcodeProject::bDoesFileExist(string fileName, string fileType){
//    
//    // grab all the strings
//    pugi::xpath_node_set source = doc.select_nodes("//string/text()");
//    
//    // if it equals something like "sourcecode.cpp.cpp" check the file name
//    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
//        pugi::xpath_node node = *it;
//        if (strcmp(node.node().value(), fileType.c_str()) == 0){
//            pugi::xpath_node_set set = node.node().parent().parent().select_nodes("string[4]");
//            set.begin()->node().first_child().value();
//        }
//    }
//    return false;
//}


string generateUUID(string input){
    
    std::string passphrase("openFrameworks"); // HMAC needs a passphrase

    HMACEngine<MD5Engine> hmac(passphrase); // we'll compute a MD5 Hash
    hmac.update(input);

    const DigestEngine::Digest& digest = hmac.digest(); // finish HMAC computation and obtain digest
    std::string digestString(DigestEngine::digestToHex(digest)); // convert to a string of hexadecimal numbers

    return digestString;	


}



void xcodeProject::addSrc(string srcFile, string folder){
    
    
    
    // find the extension for the file that's passed in. 
    
    size_t found = srcFile.find_last_of(".");
    string ext = srcFile.substr(found+1);
    
    
    // based on the extension make some choices about what to do:
    
    bool addToResources = true;
    bool addToBuild = true;
    string fileKind = "";
    bool bAddFolder = true;
    
    if( ext == "cpp" ){
        fileKind = "sourcecode.cpp.cpp";
        addToResources = false;													
    }
    if( ext == "c" ){
        fileKind = "sourcecode.c.c";				
        addToResources = false;															
    }
    if(ext == "h"){
        fileKind = "sourcecode.c.h";
        addToBuild = false;
        addToResources = false;													
    }
    if(ext == "mm" || ext == "m"){
        addToResources = false;						
        fileKind = "sourcecode.cpp.objcpp";
    }
    if(ext == "xib"){
        addToBuild	= false;
    }
    if (folder == "src"){
        bAddFolder = false;
    }
    
    //-----------------------------------------------------------------
    // (A) make a FILE REF
    //-----------------------------------------------------------------
    
    string pbxfileref = string(PBXFileReference);
    string UUID = generateUUID(srcFile);   // replace with theo's smarter system. 
    findandreplace( pbxfileref, "FILENAME", srcFile);
    findandreplace( pbxfileref, "FILEPATH", folder + "/" + srcFile);
    findandreplace( pbxfileref, "FILETYPE", fileKind);
    findandreplace( pbxfileref, "FILEUUID", UUID);
    
    pugi::xml_document fileRefDoc;
    pugi::xml_parse_result result = fileRefDoc.load_buffer(pbxfileref.c_str(), strlen(pbxfileref.c_str()));
    
    // insert it at <plist><dict><dict>
    doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(fileRefDoc.first_child().next_sibling());   // UUID FIRST
    doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(fileRefDoc.first_child());                  // DICT SECOND
    
    //-----------------------------------------------------------------
    // (B) BUILD REF
    //-----------------------------------------------------------------
    
    if (addToBuild == true){
        
        string buildUUID = generateUUID(srcFile + "-build");
        string pbxbuildfile = string(PBXBuildFile);
        findandreplace( pbxbuildfile, "FILEUUID", UUID);
        findandreplace( pbxbuildfile, "BUILDUUID", buildUUID);
        fileRefDoc.load_buffer(pbxbuildfile.c_str(), strlen(pbxbuildfile.c_str()));
        doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(fileRefDoc.first_child().next_sibling());   // UUID FIRST
        doc.select_single_node("/plist[1]/dict[1]/dict[2]").node().prepend_copy(fileRefDoc.first_child());                  // DICT SECOND
        
        // add it to the build array. 
        pugi::xml_node array;
        findArrayForUUID("E4B69E200A3A1BDC003C02F2", array);    // this is the build array (all build refs get added here)
        array.append_child("string").append_child(pugi::node_pcdata).set_value(buildUUID.c_str());
    }
    
    //-----------------------------------------------------------------
    // (C) resrouces 
    //-----------------------------------------------------------------
    
    
    if (addToResources == true){
        // no idea where resources go
    }
    
    
    //-----------------------------------------------------------------
    // (D) folder
    //-----------------------------------------------------------------
    
    // now, do we need to add this to src, or somewhere else?
    
    if (bAddFolder == true){
        
        vector < string > folders = ofSplitString(folder, "/");
        
        int howManyLevelsDeep = folders.size();
        int howManyLevelsExist = 0;
        
        //pugi::xml_node node = findOrMakeFolderSet( folders );
        
    } else {
        
        //pugi::xml_node array;
        //findArrayForUUID("E4B69E200A3A1BDC003C02F2", array);       
        //array.append_child("string").append_child(pugi::node_pcdata).set_value(buildUUID.c_str());

        
    }
} 


// todo: these three have very duplicate code... please fix up a bit. 

void xcodeProject::addInclude(string includeName){
    
    
    
    char query[255];
    sprintf(query, "//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'HEADER_SEARCH_PATHS')]/following-sibling::node()[1]");
    pugi::xpath_node_set headerArray = doc.select_nodes(query);

    if (headerArray.size() > 0){
        //cout << "adding header " << endl;
        
        for (pugi::xpath_node_set::const_iterator it = headerArray.begin(); it != headerArray.end(); ++it){
            pugi::xpath_node node = *it;
            //node.node().print(std::cout);
            node.node().append_child("string").append_child(pugi::node_pcdata).set_value(includeName.c_str());
        }
        
    } else {
        
        query[255];
        sprintf(query, "key[contains(.,'baseConfigurationReference')]/parent::node()");
        pugi::xpath_node_set dictArray = doc.select_nodes(query);
        for (pugi::xpath_node_set::const_iterator it = dictArray.begin(); it != dictArray.end(); ++it){
            pugi::xpath_node node = *it;
            
            // todo: this needs to get done
            //node.node().append_child("string").append_child(pugi::node_pcdata).set_value(includeName.c_str());
        }
    }

}  
        
        
void xcodeProject::addLibrary(string libraryName){
    
    // split apart path or not?
    
    
    char query[255];
    sprintf(query, "//key[contains(.,'baseConfigurationReference')]/parent::node()//key[contains(.,'OTHER_LDFLAGS')]/following-sibling::node()[1]");
    pugi::xpath_node_set headerArray = doc.select_nodes(query);
    
    
    if (headerArray.size() > 0){
        cout << "adding library " << endl;
        
        for (pugi::xpath_node_set::const_iterator it = headerArray.begin(); it != headerArray.end(); ++it){
            
            pugi::xpath_node node = *it;
            //node.node().print(std::cout);
            node.node().append_child("string").append_child(pugi::node_pcdata).set_value(libraryName.c_str());
        }
        
    } else {
        
        query[255];
        sprintf(query, "key[contains(.,'baseConfigurationReference')]/parent::node()");
        pugi::xpath_node_set dictArray = doc.select_nodes(query);
        for (pugi::xpath_node_set::const_iterator it = dictArray.begin(); it != dictArray.end(); ++it){
            pugi::xpath_node node = *it;
            
            // todo: this needs to get done
            //node.node().append_child("string").append_child(pugi::node_pcdata).set_value(includeName.c_str());
        }
    }
    
}

void xcodeProject::addAddon(ofAddon & addon){

}

string xcodeProject::getName(){

}

string xcodeProject::getPath(){

}
