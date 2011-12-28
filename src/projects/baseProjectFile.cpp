
#include "baseProjectFile.h"

bool baseProjectFile::doesTagAndAttributeExist(pugi::xml_document & doc, string tag, string attribute, string newValue){
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

void baseProjectFile::appendValue(pugi::xml_document & doc, string tag, string attribute, string newValue){
    
    if (!doesTagAndAttributeExist(doc, tag, attribute, newValue)){
        // otherwise, add it please: 
        char xpathExpression[1024];
        sprintf(xpathExpression, "//%s[@%s]", tag.c_str(), attribute.c_str());
        pugi::xpath_node_set add = doc.select_nodes(xpathExpression);
        pugi::xml_node node = add[add.size()-1].node();
        pugi::xml_node nodeAdded = node.parent().append_copy(node);
        nodeAdded.attribute(attribute.c_str()).set_value(newValue.c_str());
    }
}
