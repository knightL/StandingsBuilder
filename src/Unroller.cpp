/*
 * Unroller.cpp
 *
 *      Author: knightl
 */

#include "Unroller.h"
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <algorithm>
#include <ctime>
#include "XMLParser.h"
#include "config.h"
#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */
using namespace std;

const int MAXIMAL_TIME=(1U<<31)-1;

static std::string getProblemName(int id)
{
	if(1<=id && id<=26)
		return std::string(1,'A'+id-1);
	else
		return getProblemName((id-1)/26+1) + std::string(1,'A'+(id-1)%26);
}

_Unroller::_Unroller() {
	contest=NULL;
	curtime=0;
	problem_count=-1;
	refresh_rate=5*60;
}

int _Unroller::parse_problem_count(const char* str) const
{
	int res=0;
	if('0'<=str[0] && str[0]<='9')
	{

		for(int i=0;str[i];i++)
			if('0'<=str[i] && str[i]<='9')
			{
				res=res*10+str[i]-'0';
			}
			else
			{
				printf("Failed to read \"ProblemCount\". Got %s\n",str);
				exit(1);
			}
	}
	else
	{
		for(int i=0;str[i];i++)
			if('A'<=str[i] && str[i]<='Z')
			{
				res=res*26+str[i]-'A';
			}
			else
			{
				printf("Failed to read \"ProblemCount\". Got %s\n",str);
				exit(1);
			}
		res++;
	}
	return res;
}

void _Unroller::init(int argc, char** argv)
{
	printf("Standings Builder v" VERSION "\n");

	// program should run with one single argument - path to XML config
	char path[300];
	if(argc==2)
		strcpy(path,argv[1]);
	else
	{
		//print help and exit
		print_help(PACKAGE_NAME);
		exit(1);
	}

	//read config
	xmlDocPtr doc=xmlReadFile(path,NULL,0);
	if(!doc)
	{
		printf("Failed to open config file %s\n",path);
		exit(1);
	}
	XMLParser parser(doc);

	// read ProblemCount from config
	xmlNodePtr node=parser.findNode(parser.getChild(parser.getRoot()),"ProblemCount");
	if(!node)
	{
		printf("Failed to find \"ProblemCount\" in xml config \n");
		exit(1);
	}
	problem_count=parse_problem_count(parser.getNodeContent(node).c_str());

	if(problem_count<=0)
	{
		printf("Incorrect value for ProblemCount: %d <= 0\n", problem_count);
		exit(1);
	}

	//read path to output file from config
	node=parser.findNode(parser.getChild(parser.getRoot()),"OutputFile");
	if(!node)
	{
		printf("Failed to find \"OutputFile\" in xml config \n");
		exit(1);
	}
	output_file_name=parser.getNodeContent(node);

	//read path to script which will build output file
	node=parser.findNode(parser.getChild(parser.getRoot()),"OutputScript");
	if(!node)
	{
		printf("Failed to find \"OutputScipt\" in xml config \n");
		exit(1);
	}
	string script_file_name=parser.getNodeContent(node);

	// read refresh rate, default value 300 seconds
	node=parser.findNode(parser.getChild(parser.getRoot()),"RefreshRate");
	if(node)
		this->refresh_rate=atoi(parser.getNodeContent(node).c_str());
	else
		printf("No refresh rate provided. Using default value: %d\n",this->refresh_rate);

	printf("Output File:   %s\n",output_file_name.c_str());
	printf("Problem Count: %d (%s-%s)\n",problem_count, "A", getProblemName(problem_count).c_str());

	// read parsers and store them
	printf("Parsers:\n");
	node=parser.getChild(parser.findNode(parser.getChild(parser.getRoot()),"Parsers"));
	for(;node;node=parser.getNext(node))
	{
		std::string name=(char*) node->name;
		// find parser with this name and store it in parsers list
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

	contest=new Contest(problem_count,script_file_name);
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
		if(i!=0) puts("---");
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
#ifdef linux
	timespec delay;
#endif /* linux */
	// update all parsers every (refresh_rate) seconds
	while(1)
	{
		//update information in all parsers (time, submissions, ...)
		for(int i=0;i<(int)parsers.size();i++)
			parsers[i]->update();

		//get time from first timer that provides time
		for(int i=0;i<(int)parsers.size();i++)
			if(parsers[i]->providesTime())
			{
				curtime=parsers[i]->getTime();
				break;
			}
		
		//let all parsers update contest with newfound time
		for(int i=0;i<(int) parsers.size();i++)
			parsers[i]->updateContest(contest,parser_provides_time?curtime:MAXIMAL_TIME);

		// print standings to output file
		contest->print_standings(this->output_file_name, parser_provides_time?curtime:-1);

		// if delay is zero, end cycle
		if(refresh_rate==0)
			break;
		//make delay
#ifdef WIN32
		Sleep(refresh_rate*1000);
#elif linux
		delay.tv_sec=refresh_rate;
		delay.tv_nsec=0;
		nanosleep(&delay,NULL);
#else
#error unknown architecutre
#endif
	}
}

Contest* _Unroller::get_contest()
{
	return contest;
}

