/*
 * NEERCHtmlParser.cpp
 *
 *  Created on: 13 мая 2014
 *      Author: knightl
 */

#include "../Unroller.h"
#include "NEERCHtmlParser.h"
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <algorithm>
using namespace std;

NEERCHtmlParser::Event::Event(int time, std::string team, int id, bool accepted, int attempts):
	time(time),
	team(team),
	id(id),
	accepted(accepted),
	attempts(attempts)
{
}

bool NEERCHtmlParser::Event::operator<(const Event& ev) const
{
	return time<ev.time;
}

NEERCHtmlParser::NEERCHtmlParser(const XMLParser& config, xmlNodePtr start):Parser(config,start) {
	Unroller unroller;
	int problem_count=unroller->get_problem_count();
	bool showUnsolved=true;
	xmlNodePtr attr=(xmlNodePtr)config.findAttribute(start->properties,"HideUnsolved");
	if( attr!=NULL && !strcmp((char*)xmlNodeGetContent(attr),"Yes"))
		showUnsolved=false;

	start=(xmlNodePtr)config.findAttribute(start->properties,"Path");
	if(!start)
		printf("NEERCHtmlParser: Missing property \"Path\"\n");
	else
	{
		long size;
		string path=(char*)xmlNodeGetContent(start);
		FILE *fin=fopen(path.c_str(),"rb");
		if(!fin)
		{
			printf("Failed to open file %s\n",path.c_str());
			return;
		}

		fseek(fin, 0, SEEK_END);
		size = ftell(fin);
		fseek(fin, 0, SEEK_SET);

		char *buf=(char*)calloc(size+1,sizeof(char));
		fread(buf,1,size,fin);
		fclose(fin);

		XMLParser xml(buf);
		free(buf);
//  1 / 2  /  3  /  4  / 5/ 6/ 7    /8    /9    /0
//html/body/table/tbody/tr/td/center/table/tbody/tr
		char html_path[][20]={
				"body",
				"table",
				"tbody",
				"tr",
				"td",
				"center",
				"table",
				"tbody",
				"tr"
		};
		xmlNodePtr start=xml.getRoot();
		for(int i=0;i<(int)(sizeof(html_path)/sizeof(html_path[0]));i++)
			start=xml.findNode(xml.getChild(start),html_path[i]);
		start=xml.findNode(xml.getNext(start),"tr");
		while(start)
		{
			start=xml.findNode(xml.getNext(start),"tr");
			if(!start) break;
			xmlNodePtr cur=xml.getChild(start);
			cur=xml.getNext(cur);
			if(!cur) break;
			string name=(char*)xmlNodeGetContent(cur);
			for(int i=0;i<problem_count;i++)
			{
				cur=xml.getNext(cur);
				string t1=(char*)xmlNodeGetContent(cur);
				if(t1[0]=='-')
				{
					if(showUnsolved)
						event.push_back(Event(0,name,i,false,-atoi(t1.c_str())));
				}
				else if(t1[0]=='+')
				{
					string t2=(char*)xmlNodeGetContent(xml.getChild(xml.getChild(cur)));
					t1=t1.substr(0, t1.size() - t2.size() );
					event.push_back(Event(atoi(t2.c_str()),name,i,true,
							(t1.size()==1)?0:atoi(t1.c_str())));

				}
			}
		}
		sort(event.begin(),event.end());
		reverse(event.begin(),event.end());
	}
}

NEERCHtmlParser::~NEERCHtmlParser() {
}

void NEERCHtmlParser::update()
{
}

void NEERCHtmlParser::updateContest(Contest* contest, int time)
{
	while( !event.empty() && event.back().time<=time)
	{
		pTeam team = contest->extract_team(event.back().team) ;
		for(int i=0;i<event.back().attempts;i++)
			team->make_attempt(event.back().time, event.back().id, false);
		if(event.back().accepted)
			team->make_attempt(event.back().time, event.back().id, true);
		team->set_type(style);
		contest->add_team(team);
		event.pop_back();
	}
}

bool NEERCHtmlParser::providesTime()
{
	return false;
}

std::string NEERCHtmlParser::getName()
{
	return "NEERCHtmlParser";
}

std::string NEERCHtmlParser::getDescription()
{
	return "Necessary attributes:\n\
<Path> - path to neerc standings\n\
Optional attributes:\n\
<HideUnsolved> - [No/Yes] hide attempts on problems without AC";
}
