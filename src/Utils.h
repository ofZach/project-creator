/*
 * Utils.h
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "ofConstants.h"

#include "pugixml.hpp"


string getOFRoot();
void setOFRoot(string path);

bool doesTagAndAttributeExist(pugi::xml_document & doc, string tag, string attribute, string newValue);
pugi::xml_node appendValue(pugi::xml_document & doc, string tag, string attribute, string newValue);


void getFilesRecursively(const string & path, vector < string > & fileNames);
void getLibsRecursively(const string & path, vector < string > & libFiles, vector < string > & libLibs, string platform="" );


#endif /* UTILS_H_ */
