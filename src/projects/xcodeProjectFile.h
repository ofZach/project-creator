

#pragma once

#include "baseProjectFile.h"


class xcodeSrcFile {
    
    
public: 
    xcodeSrcFile(){
    };
    ~xcodeSrcFile(){};
    string UUID;
    string fileName;
    string filePath;
    
    string buildRefUUID;
    
    // I appear a few places. 
    
    pugi::xml_node srcNode;
    pugi::xml_node srcArrayNode;
    pugi::xml_node buildRefNode;
    pugi::xml_node buildRefArrayNode;
    
    
    
} ;


class xcodeProjectFile : public baseProjectFile {
    
public: 
    
    void loadFile(string fileName);
    void saveFile(string fileName);
    void addSrc(string srcFile);
    void addInclude(string includeName);
    void addLibrary(string libraryName);
    
    // adding src is hard, here's some help
    vector < xcodeSrcFile > srcFiles;
    void parseForSrc();
    bool findArrayForUUID(string UUID, pugi::xml_node & nodeMe);
    
};



//http://pocoproject.org/docs/Poco.UUIDGenerator.html

//"xcode/emptyExample.xcodeproj/project.pbxproj"

// to add an object (src)

// first add it here: 
// /plist[1]/dict[1]/dict[2]

/*
 <key>117A509E14AA1DEC001889C6</key>  << ----- UUID
 <dict>
 <key>fileEncoding</key>
 <string>4</string>
 <key>isa</key>
 <string>PBXFileReference</string>
 <key>lastKnownFileType</key>
 <string>sourcecode.c.h</string>
 <key>path</key>
 <string>blah.h</string>
 <key>sourceTree</key>
 <string>&lt;group&gt;</string>
 </dict>
 */

// then here:

/*
 
 <dict>
 <key>children</key>
 <array>
 <string>E4B6FCAD0C3E899E008CF71C</string>
 <string>E4EB6923138AFD0F00A09F29</string>
 <string>E4B69E1C0A3A1BDC003C02F2</string>
 <string>E4EEC9E9138DF44700A80321</string>
 <string>BB4B014C10F69532006C3DED</string>
 <string>E45BE5980E8CC70C009D7055</string>
 <string>E4B69B5B0A3A1756003C02F2</string>
 </array>
 <key>isa</key>
 <string>PBXGroup</string>
 <key>sourceTree</key>
 <string>&lt;group&gt;</string>
 </dict>

 */

// search paths: 

/*
 
 <key>LIBRARY_SEARCH_PATHS</key>
 <array>
 <string>$(inherited)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_1)</string>
*/


// linking 
// (this is missing from the default empty example, somehow)

/*
 
 //dict/buildSettings
 
 
 <dict>
 <key>buildSettings</key>
 <dict>
 <key>COPY_PHASE_STRIP</key>
 <string>YES</string>
 <key>FRAMEWORK_SEARCH_PATHS</key>
 <array>
 <string>$(inherited)</string>
 <string>$(FRAMEWORK_SEARCH_PATHS_QUOTED_FOR_TARGET_1)</string>
 </array>
 <key>FRAMEWORK_SEARCH_PATHS_QUOTED_FOR_TARGET_1</key>
 <string>"$(SRCROOT)/../../../libs/glut/lib/osx"</string>
 <key>GCC_ENABLE_FIX_AND_CONTINUE</key>
 <string>NO</string>
 <key>GCC_GENERATE_DEBUGGING_SYMBOLS</key>
 <string>NO</string>
 <key>GCC_MODEL_TUNING</key>
 <string>G4</string>
 <key>GCC_PRECOMPILE_PREFIX_HEADER</key>
 <string>YES</string>
 <key>GCC_PREFIX_HEADER</key>
 <string>$(SYSTEM_LIBRARY_DIR)/Frameworks/Carbon.framework/Headers/Carbon.h</string>
 <key>INFOPLIST_FILE</key>
 <string>openFrameworks-Info.plist</string>
 <key>INSTALL_PATH</key>
 <string>$(HOME)/Applications</string>
 <key>LIBRARY_SEARCH_PATHS</key>
 <array>
 <string>$(inherited)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_1)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_2)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_3)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_4)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_5)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_6)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_7)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_8)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_9)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_10)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_11)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_12)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_13)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_14)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_15)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_2)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_1)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_3)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_7)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_8)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_9)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_10)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_11)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_12)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_13)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_16)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_17)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_18)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_19)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_20)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_21)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_22)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_23)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_24)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_25)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_26)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_27)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_28)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_29)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_30)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_31)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_32)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_33)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_34)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_35)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_36)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_37)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_38)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_39)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_40)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_41)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_42)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_43)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_44)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_45)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_46)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_47)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_48)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_49)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_50)</string>
 <string>$(LIBRARY_SEARCH_PATHS_QUOTED_FOR_TARGET_51)</string>
 </array>
 <key>OTHER_LDFLAGS</key>
 <array>
 <string>$(OF_CORE_LIBS)</string>
 <string>testLib.lib</string>
 </array>
 
 */

