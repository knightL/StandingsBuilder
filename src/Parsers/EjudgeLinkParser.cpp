/*
 * EjudgeLinkParser.cpp
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#include "EjudgeLinkParser.h"
#include "../GetPage.h"
using namespace std;

EjudgeLinkParser::EjudgeLinkParser(const XMLParser& config, xmlNodePtr start):
		EjudgeParser(config,start) {
	start=(xmlNodePtr)config.findAttribute(start->properties,"URL");
	if(!start)
		printf("EjudgeLinkParser: Missing property \"URL\"\n");
	else
		url=(char*)xmlNodeGetContent(start);
}

EjudgeLinkParser::~EjudgeLinkParser() {
}

void EjudgeLinkParser::update()
{
	if(xml) delete xml;
	MemoryStruct mem=DownloadURL(url.c_str());
	if(!mem.memory) return;
	xml = new XMLParser(mem.memory);
	free(mem.memory);

	xmlNodePtr root=xml->getRoot();
	xmlNodePtr body=xml->findNode(xml->getChild(root), "body");
	xmlNodePtr state=xml->findNode(xml->getChild(body), "h1");
	if(state)
	{
		char* description= (char*)xmlNodeGetContent(state);
		int i;
		for(i=0;description[i];i++)
			if(description[i]=='[')
			{
				i++;
				break;
			}
		int h,m,s;
		if(sscanf(description+i,"%d:%d:%d",&h,&m,&s)==3)
			curtime=m+60*h;
		else
			printf("Failed to read time");
	}
}

std::string EjudgeLinkParser::getName()
{
	return "EjudgeLinkParser";
}

std::string EjudgeLinkParser::getDescription()
{
	return
"Necessary attributes:\n\
<URL> - link to ejudge standings\n\
Optional attributes:\n\
<Timeless> - [No/Yes] whether standings have time of submission or not";
}
