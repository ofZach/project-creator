#pragma once

#include "ofMain.h"




class ofAddon {
    
public: 

    // this is source files:
    map < string, string > filesToFolders;      //the addons has had, for each file, 
                                                //sometimes a listing of what folder to put it in, such as "addons/ofxOsc/src"
        
    vector < string > srcFilesToAdd;
    vector < string > libsToAdd;
    vector < string > includePathsToAdd;
    
};



class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
