#include "testApp.h"
#include "pugixml.hpp"



//--------------------------------------------------------------
void testApp::setup(){

    
    //AP.loadAddons();
    
    /*
    CBP.loadFile("codeblocks_windows/emptyExample.cbp");
    
    CBP.addSrc( "src/testApp.cpp");
    CBP.addSrc( "src/testApp23.cpp");
    CBP.addSrc( "src/testApp232342.cpp");
    CBP.addInclude( "../../../sdfasdf.h");
    CBP.addLibrary( "../../../sdfasdf.lib");
    
    CBP.saveFile("codeblocks_windows/emptyExample2.cbp");
    
    
    printf("------ \n");
    VSP.loadFile("visualstudio_windows/emptyExample.vcxproj");
    VSP.addInclude("test");
    VSP.addLibrary("../../../addons/opencv/opencv.a");
    VSP.addSrc("src/testApp.h");
    VSP.addSrc("src/blah.h");
    VSP.addSrc("src/blah.cpp");
    VSP.saveFile("visualstudio_windows/emptyExample2.vcxproj");
    
    */
    XCP.loadFile("xcode/emptyExample.xcodeproj/project.pbxproj");
    XCP.addSrc("src/blah.h");
     XCP.saveFile("xcode/emptyExample.xcodeproj/project2.pbxproj");
    
//    
//    addSrc(doc, "src/testApp.cpp");
//    addSrc(doc, "testApp3.cpp");
//    addSrc(doc, "testApp4.cpp");
//    addInclude(doc, "../../../addons/test.cpp");
//    addInclude(doc, "../../../addons/test.h");
//    addLibrary(doc, "../../../addons/blah.lib");
//    
//    
//    
//    
//    
//    
//    
//    doc.save_file(ofToDataPath("codeblocks_windows/test.cbp").c_str());
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

