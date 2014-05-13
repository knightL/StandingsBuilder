/*
 * XMLParser.cpp
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#include "XMLParser.h"
#include "libxml/HTMLparser.h"
#include <cstring>

XMLParser::XMLParser(char* buf) {
	int top=0;
	int len=strlen(buf);
//	for(int i=0;i+1<len;i++)
//	{
//		if(buf[i]=='<' && buf[i+1]=='!')
//		{
//			buf[i]=buf[i+1]=' ';
//			int j=i;
//			while(j<len && buf[j]!='>')
//			{
//				buf[j]=' ';
//				j++;
//			}
//			if(j<len)
//				buf[j]=' ';
//		}
//	}
	for(int i=0;i<len;i++)
	{
		if(!strncmp(&buf[i],"<html>",6))
		{
			top=i;
			break;
		}
	}
//	for(int i=top;i<len;i++)
//	{
//		if(buf[i]=='&')
//		{
//			int j;
//			for(j=i;j<len && buf[j]!=';';j++)
//				buf[j]=' ';
//			buf[j]=' ';
//		}
//	}
//	doc=xmlReadMemory(buf+top,len-top,NULL,NULL,0);
	doc=(xmlDocPtr)htmlReadMemory(buf+top,len-top,NULL,NULL,0);
	if(doc)
	{
		root=xmlDocGetRootElement(doc);
	}
	else
	{
		root=NULL;
		printf("Failed to parse HTML\n");
	}
}

XMLParser::XMLParser(xmlDocPtr doc)
{
	this->doc=doc;
	root=xmlDocGetRootElement(doc);
}

xmlNode* XMLParser::getRoot() const
{
	return root;
}

xmlNode* XMLParser::getChild(xmlNode* cur) const
{
	if(!cur)
	{
		fprintf(stderr,"Warning: Got Empty Node\n");
		return NULL;
	}
	return xmlFirstElementChild(cur);
}

xmlNode* XMLParser::getNext(xmlNode* cur) const
{
	if(!cur)
	{
		fprintf(stderr,"Warning: Got Empty Node\n");
		return NULL;
	}
	return xmlNextElementSibling(cur);
}

xmlNode* XMLParser::findNode(xmlNode* start, std::string nodeName) const
{
	while(start && strcmp(nodeName.c_str(), (char*) start->name))
		start=getNext(start);
	return start;
}

xmlAttr* XMLParser::findAttribute(xmlAttrPtr start, std::string AttrName) const
{
	while(start && strcmp(AttrName.c_str(), (char*) start->name))
		start=start->next;
	return start;
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

