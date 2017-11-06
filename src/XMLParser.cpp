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

xmlDoc* XMLParser::getDoc() const
{
	return this->doc;
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

xmlNode* XMLParser::walkPath(xmlNodePtr start, const std::vector<std::string>& path) const
{
	xmlNodePtr cur=start;
	for(const std::string& s: path)
		if(s==">")
			cur=this->getChild(cur);
		else if(s=="+")
			cur=this->getNext(cur);
		else
			cur=this->findNode(cur, s);
	return cur;
}

xmlAttrPtr XMLParser::getAttributeList(xmlNodePtr node) const
{
	return node->properties;
}

xmlAttrPtr XMLParser::getNextAttribute(xmlAttrPtr node) const
{
	return node->next;
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

std::string XMLParser::getAttributeContent(xmlNodePtr start, std::string  attrName) const
{
	xmlAttrPtr ptr=findAttribute(start->properties, attrName);
	return ptr? getNodeContent((xmlNodePtr)ptr): "";
}

std::string XMLParser::getCurrentAttributeContent(xmlAttrPtr attr) const
{
	if(!attr)
	{
		fprintf(stderr,"Warning: Got Empty Attribute\n");
		return "";
	}
	xmlChar* cur=xmlNodeGetContent((xmlNodePtr)attr);
	if(!cur)
	{
		fprintf(stderr,"Warning: No content found\n");
		return "";
	}
	std::string res=(char*)cur;
	this->free(cur);
	return res;
}

std::string XMLParser::getNodeContent(xmlNodePtr node) const
{
	if(!node)
	{
		fprintf(stderr,"Warning: Got Empty Node\n");
		return "";
	}
	xmlChar* cur=xmlNodeGetContent(node);
	if(!cur)
	{
		fprintf(stderr,"Warning: No content found\n");
		return "";
	}
	std::string res=(char*)cur;
	this->free(cur);
	return res;
}

void XMLParser::free(xmlChar* data) const
{
	xmlFree(data);
}

XMLParser::~XMLParser()
{
	xmlFreeDoc(doc);
}

void XMLParser::printTree(xmlNodePtr start, int depth) const
{
	for(int i=0;i<depth;i++)
		printf("%d",(i+1)%10);
	printf("\"%s\":\"%s\"\n",(start->name),this->getNodeContent(start).c_str());
	xmlNodePtr i = this->getChild(start);
	for(;i;i= this->getNext(i))
	{
		printTree(i,depth+1);
	}
}

