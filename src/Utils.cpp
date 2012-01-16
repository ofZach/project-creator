/*
 * Utils.cpp
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#include "Utils.h"

#include <algorithm>
#include <Poco/DirectoryIterator.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>

using namespace Poco;

#include "ofUtils.h"


void findandreplace( std::string& tInput, std::string tFind, std::string tReplace ) { 
	size_t uPos = 0; 
	size_t uFindLen = tFind.length(); 
	size_t uReplaceLen = tReplace.length();
    
	if( uFindLen == 0 ){
		return;
	}
    
	for( ;(uPos = tInput.find( tFind, uPos )) != std::string::npos; ){
		tInput.replace( uPos, uFindLen, tReplace );
		uPos += uReplaceLen;
	}	
    
}


bool doesTagAndAttributeExist(pugi::xml_document & doc, string tag, string attribute, string newValue){
    char xpathExpressionExists[1024];
    sprintf(xpathExpressionExists, "//%s[@%s='%s']", tag.c_str(), attribute.c_str(), newValue.c_str());
    cout <<xpathExpressionExists <<endl;
    pugi::xpath_node_set set = doc.select_nodes(xpathExpressionExists);
    if (set.size() != 0){
        return true;
    } else {
        return false;
    }
}

pugi::xml_node appendValue(pugi::xml_document & doc, string tag, string attribute, string newValue){

    if (!doesTagAndAttributeExist(doc, tag, attribute, newValue)){
        // otherwise, add it please:
        char xpathExpression[1024];
        sprintf(xpathExpression, "//%s[@%s]", tag.c_str(), attribute.c_str());
        pugi::xpath_node_set add = doc.select_nodes(xpathExpression);
        pugi::xml_node node = add[add.size()-1].node();
        pugi::xml_node nodeAdded = node.parent().append_copy(node);
        nodeAdded.attribute(attribute.c_str()).set_value(newValue.c_str());
        return nodeAdded;
    }else{
    	return pugi::xml_node();
    }

}


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


void getLibsRecursively(const string & path, vector < string > & libFiles, vector < string > & libLibs, string platform ){
       DirectoryIterator end;
        for (DirectoryIterator it(path); it != end; ++it){
            if (!it->isDirectory()){
            	string ext = ofFilePath::getFileExt(it->path());
            	vector<string> splittedPath = ofSplitString(ofFilePath::getEnclosingDirectory(it->path()),"/");

                if (ext == "a" || ext == "lib" || ext == "dylib" || ext == "so"){

                	if(platform!=""){
                		bool platformFound = false;
                		for(int i=0;i<(int)splittedPath.size();i++){
                			if(splittedPath[i]==platform){
                				platformFound = true;
                				break;
                			}
                		}
                		if(!platformFound){
                			continue;
                		}
                	}
                    libLibs.push_back(it->path());
                } else if (ext == "h" || ext == "hpp" || ext == "c" || ext == "cpp" || ext == "cc"){
                    libFiles.push_back(it->path());
                }
            }

            if (it->isDirectory()){
                getLibsRecursively(it->path(), libFiles, libLibs, platform);
            }
        }

}



void fixSlashOrder(string & toFix){
    std::replace(toFix.begin(), toFix.end(),'/', '\\');
}


string unsplitString (vector < string > strings, string deliminator ){
    string result;
    for (int i = 0; i < (int)strings.size(); i++){
        if (i != 0) result += deliminator;
        result += strings[i];
    }
    return result;
}


static string OFRoot = "../../..";

string getOFRoot(){
	return ofFilePath::removeTrailingSlash(OFRoot);
}


void setOFRoot(string path){
	OFRoot = path;
}
