/*
 * Unroller.cpp
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#include "../config.h"
#include "Unroller.h"
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <algorithm>
#include <ctime>
#include "GetPage.h"
#include "XMLParser.h"
using namespace std;

const int MAXIMAL_TIME=(1U<<31)-1;

_Unroller::_Unroller() {
	contest=NULL;
	curtime=0;
	problem_count=-1;
	timeout=5*60;
}

void _Unroller::init(int argc, char** argv)
{
	printf("Standings Builder v"VERSION"\n");
	char path[300];
	if(argc>1)
		strcpy(path,argv[1]);
	else
	{
		print_help(PACKAGE_NAME);
		exit(1);
	}
	xmlDocPtr doc=xmlReadFile(path,NULL,0);
	if(!doc)
	{
		printf("Failed to open config file %s\n",path);
		exit(1);
	}
	XMLParser parser(doc);
	xmlNodePtr node=parser.findNode(parser.getChild(parser.getRoot()),"ProblemCount");
	if(!node)
	{
		printf("Failed to find \"ProblemCount\" in xml config \n");
		exit(1);
	}
	problem_count=atoi((char*)xmlNodeGetContent(node));
	curtime=0;

	assert(problem_count>=0);

	node=parser.findNode(parser.getChild(parser.getRoot()),"OutputFile");
	if(!node)
	{
		printf("Failed to find \"OutputFile\" in xml config \n");
		exit(1);
	}
	output_file_name=(char*)xmlNodeGetContent(node);

	node=parser.findNode(parser.getChild(parser.getRoot()),"Timeout");
	if(node)
	{
		this->timeout=atoi((char*)xmlNodeGetContent(node));
	}

	printf("Output File:   %s\n",output_file_name.c_str());
	printf("Problem Count: %d (%c-%c)\n",problem_count, 'A','A'+problem_count-1);
	printf("Parsers:\n");
	node=parser.getChild(parser.findNode(parser.getChild(parser.getRoot()),"Parsers"));
	for(;node;node=parser.getNext(node))
	{
		std::string name=(char*) node->name;
		int i;
		for(i=0;i<(int)builders.size();i++)
			if(name==builders[i]->getName())
			{
				this->parsers.push_back(builders[i]->build(parser,node));
				break;
			}
		if(i==(int)builders.size())
			printf("Unknown parser %s\n",name.c_str());
		else
			printf("%s\n",name.c_str());
	}

	contest=new Contest(problem_count);
}

_Unroller::~_Unroller() {
	delete contest;
}

void _Unroller::print_help(string name)
{
	printf("Usage: %s <config.xml>\n",name.c_str());
	printf("Parsers:\n");
	for(int i=0;i<(int)builders.size();i++)
	{
		if(i) puts("---");
		printf("%s\n",builders[i]->getName().c_str());
		printf("%s\n",builders[i]->getDescription().c_str());
	}
}

int _Unroller::get_problem_count() const
{
	return problem_count;
}

void _Unroller::run()
{
	bool parser_provides_time=false;
	for(int i=0;i<(int)parsers.size();i++)
		parser_provides_time|=parsers[i]->providesTime();
	if(!parser_provides_time)
		printf("No parser provides time. Maximal time will be used\n");
	printf("Running...\n");
	timespec delay;
	while(1)
	{
		for(int i=0;i<(int)parsers.size();i++)
			parsers[i]->update();
		for(int i=0;i<(int)parsers.size();i++)
		{
			if(parsers[i]->providesTime())
			{
				curtime=parsers[i]->getTime();
				break;
			}
		}
		for(int i=0;i<(int) parsers.size();i++)
			parsers[i]->updateContest(contest,parser_provides_time?curtime:MAXIMAL_TIME);


		contest->print_standings(this->output_file_name,parser_provides_time?curtime:0);
		delay.tv_sec=timeout;
		delay.tv_nsec=0;
		nanosleep(&delay,NULL);
	}
}

