

#pragma once

#include "baseProject.h"



class xcodeProject : public baseProject {
    
public: 
    
    xcodeProject(){};
    
    bool load(string path);
    bool create(string path){}; // not done yet. 
	bool save(string path);

    void addSrc(string srcFile, string folder);
    void addInclude(string includeName);
    void addLibrary(string libraryName);

	void addAddon(ofAddon & addon);


    
    pugi::xml_node findOrMakeFolderSet( vector < string > folders );
    
    
    
	string getName();
	string getPath();

    
    pugi::xml_node insertPoint;         // where are we inserting items (at the second dict tag,
                                        // /plist[1]/dict[1]/dict[2])
    
   bool findArrayForUUID(string UUID, pugi::xml_node & nodeMe);
    
};



//http://pocoproject.org/docs/Poco.UUIDGenerator.html


// search paths: 




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



