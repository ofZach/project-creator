#include "testApp.h"
#include "Utils.h"

//--------------------------------------------------------------
void testApp::setup(){
	setOFRoot("/home/arturo/Escritorio/openFrameworks");

	ofSetLogLevel(CBLinuxProject::LOG_NAME,OF_LOG_VERBOSE);
	linuxProject.create(getOFRoot()+"/apps/testApp/testCreatorLinux");
	ofAddon addon(getOFRoot()+"/addons/ofxOpenCv","linux");
	linuxProject.addAddon(addon);

	winProject.create(getOFRoot()+"/apps/testApp/testCreatorWin");
	addon.fromFS(getOFRoot()+"/addons/ofxOpenCv","win_cb");
	winProject.addAddon(addon);

	vsProject.create(getOFRoot()+"/apps/testApp/testCreatorVS");
	addon.fromFS(getOFRoot()+"/addons/ofxOpenCv","vs2010");
	vsProject.addAddon(addon);
	vsProject.save(getOFRoot()+"/apps/testApp/testCreatorVS/testCreatorVS.vcxproj");

	xcProject.load("xcode/emptyExample.xcodeproj/project.pbxproj");
	xcProject.addSrc("src/blah.h","src");
	xcProject.save("xcode/emptyExample.xcodeproj/project2.pbxproj");
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

