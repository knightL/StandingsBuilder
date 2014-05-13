/*
 * XMLParser.h
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include<string>
#include<vector>
#include<libxml/parser.h>
#include<libxml/tree.h>


class XMLParser {
	xmlDoc* doc;
	xmlNode* root;
public:
	XMLParser(char* buf);
	XMLParser(xmlDoc* doc);
	~XMLParser();
	xmlNode* getRoot() const;
	xmlNode* getChild(xmlNode* cur) const;
	xmlNode* getNext(xmlNode* cur) const;
	xmlNode* findNode(xmlNode* start, std::string nodeName) const;
	xmlAttr* findAttribute(xmlAttrPtr start, std::string attrName ) const;
	void printTree(xmlNodePtr start, int depth=0) const;
};

#endif /* HTMLPARSER_H_ */
