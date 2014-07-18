/*
 * XMLParser.cpp
 *
 *      Author: knightl
 */

#include "XMLParser.h"
#include <cstring>


XMLParser::XMLParser(xmlDocPtr doc)
{
	this->doc=doc;
	root=xmlDocGetRootElement(doc);
}

xmlNode* XMLParser::getRoot() const
{
	return root;
}

xmlNode* XMLParser::getChild(xmlNodePtr cur) const
{
	if(!cur)
	{
		fprintf(stderr,"Warning: Got Empty Node\n");
		return NULL;
	}
	return xmlFirstElementChild(cur);
}

xmlNode* XMLParser::getNext(xmlNodePtr cur) const
{
	if(!cur)
	{
		fprintf(stderr,"Warning: Got Empty Node\n");
		return NULL;
	}
	return xmlNextElementSibling(cur);
}

xmlNode* XMLParser::findNode(xmlNodePtr start, std::string nodeName) const
{
	while(start && strcmp(nodeName.c_str(), (char*) start->name))
		start=getNext(start);
	return start;
}

xmlAttrPtr XMLParser::findAttribute(xmlAttrPtr start, std::string AttrName) const
{
	while(start && strcmp(AttrName.c_str(), (char*) start->name))
		start=start->next;
	return start;
}

bool XMLParser::haveAttribute(xmlNodePtr start, std::string attrName) const
{
	return findAttribute(start->properties, attrName)!=NULL;
}

xmlChar* XMLParser::getAttributeContent(xmlNodePtr start, std::string  attrName) const
{
	xmlAttrPtr ptr=findAttribute(start->properties, attrName);
	return ptr? xmlNodeGetContent((xmlNodePtr)ptr): NULL;
}

XMLParser::~XMLParser()
{
	xmlFreeDoc(doc);
}

void XMLParser::printTree(xmlNodePtr start, int depth) const
{
	for(int i=0;i<depth;i++)
		printf("%d",(i+1)%10);
	printf("\"%s\":\"%s\"\n",(start->name),xmlNodeGetContent(start));
	xmlNodePtr i=getChild(start);
	for(;i;i=getNext(i))
	{
		printTree(i,depth+1);
	}
}

