/*
 * CBLinuxProject.h
 *
 *  Created on: 28/12/2011
 *      Author: arturo
 */

#ifndef CBLINUXPROJECT_H_
#define CBLINUXPROJECT_H_

#include "ofConstants.h"
#include "pugixml.hpp"
#include "ofAddon.h"

class CBLinuxProject {
public:
	CBLinuxProject();

	bool load(string path);
	bool create(string path);

	void addSrc(string srcName, string folder);
	void addInclude(string includeName);
	void addLibrary(string libraryName);

	void addAddon(ofAddon & addon);

	static string LOG_NAME;

private:
	void parseAddons();

	pugi::xml_document doc;
	string projectDir, projectName;
	vector<ofAddon> addons;
};

#endif /* CBLINUXPROJECT_H_ */
