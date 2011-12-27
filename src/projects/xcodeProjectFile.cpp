


#include "xcodeProjectFile.h"


#include <iostream>

const char* node_types[] =
{
    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
};

//[code_traverse_walker_impl
struct simple_walker: pugi::xml_tree_walker
{
    virtual bool for_each(pugi::xml_node& node)
    {
        for (int i = 0; i < depth(); ++i) std::cout << "  "; // indentation
        
        std::cout << node_types[node.type()] << ": name='" << node.name() << "', value='" << node.value() << "'\n";
        
        return true; // continue traversal
    }
};
//]




void xcodeProjectFile::loadFile(string fileName){
    pugi::xml_parse_result result = doc.load_file(ofToDataPath(fileName).c_str());
    // todo check result here
    bLoaded = true;
}  





void xcodeProjectFile::saveFile(string fileName){
    doc.save_file(ofToDataPath(fileName).c_str());
}  





void xcodeProjectFile::addSrc(string srcFile){
    
    
    pugi::xpath_node_set source = doc.select_nodes("//string/text()");
    for (pugi::xpath_node_set::const_iterator it = source.begin(); it != source.end(); ++it){
        
        pugi::xpath_node node = *it;
        
        if (strcmp(node.node().value(),"sourcecode.cpp.cpp") == 0 || 
            strcmp(node.node().value(),"sourcecode.c.h") == 0 ){
            printf("------- \n");
            
            // this outputs a node.  useful for checking stuff.
            //simple_walker walker;
            //node.node().parent().parent().traverse(walker);
            //node.node().parent().parent().previous_sibling().traverse(walker);
            
            string UUID = string(node.node().parent().parent().previous_sibling().first_child().value());
            pugi::xpath_node_set set = node.node().parent().parent().select_nodes("string[4]");
            cout << UUID << " " << set.begin()->node().first_child().value() << endl;
            set = node.node().parent().parent().select_nodes("string[5]");
            cout << "path : " << " " << set.begin()->node().first_child().value() << endl;
            
        }
    }
   
    
   
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

void xcodeProjectFile::addInclude(string includeName){
    
}  
void xcodeProjectFile::addLibrary(string libraryName){
        
}
