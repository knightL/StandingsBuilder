/*
 * EjudgeFileParser.cpp
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#include "EjudgeFileParser.h"

#include<cstdio>

EjudgeFileParser::EjudgeFileParser(const XMLParser& config, xmlNodePtr start):
	EjudgeParser(config,start)
{
	start=(xmlNodePtr)config.findAttribute(start->properties,"Path");
	if(!start)
		printf("EjudgePathParser: Missing property \"Path\"\n");
	else
		path=(char*)xmlNodeGetContent(start);
}

EjudgeFileParser::~EjudgeFileParser() {
}

void EjudgeFileParser::update()
{
	if(xml) delete xml;
	FILE *fin=fopen(path.c_str(),"rb");
	if(!fin)
	{
		printf("Failed to open file %s\n",path.c_str());
		return;
	}

	long size;

	fseek(fin, 0, SEEK_END);
	size = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	char *buf=(char*)calloc(size+1,sizeof(char));
	fread(buf,1,size,fin);
	fclose(fin);

	xml = new XMLParser(buf);
	free(buf);

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

std::string EjudgeFileParser::getName()
{
	return "EjudgeFileParser";
}

std::string EjudgeFileParser::getDescription()
{
	return "Necessary attributes:\n\
<Path> - path to ejudge standings\n\
Optional attributes:\n\
<Timeless> - [No/Yes] whether standings have time of submission or not";
}
