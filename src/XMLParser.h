/*
 * XMLParser.h
 *
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
	XMLParser(xmlDocPtr doc);
	~XMLParser();
	xmlDoc* getDoc() const;
	xmlNode* getRoot() const;
	xmlNode* getChild(xmlNodePtr cur) const;
	xmlNode* getNext(xmlNodePtr cur) const;
	xmlNode* findNode(xmlNodePtr start, std::string nodeName) const;

	/*
	 * Walks from node start along the path.
	 * ">" means enter subtree
	 * "+" means get next node
	 * other string means "find next node with such name"
	 */
	xmlNode* walkPath(xmlNodePtr start, const std::vector<std::string>& path) const;
	xmlAttr* findAttribute(xmlAttrPtr start, std::string attrName ) const;
	bool haveAttribute(xmlNodePtr start, std::string) const;
	xmlChar* getAttributeContent(xmlNodePtr start, std::string) const;
	std::string getNodeContent(xmlNodePtr node) const;
	void printTree(xmlNodePtr start, int depth=0) const;
	void free(xmlChar* data) const;
};

#endif /* HTMLPARSER_H_ */
