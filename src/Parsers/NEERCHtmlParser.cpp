/*
 * NEERCHtmlParser.cpp
 *
 *      Author: knightl
 */

#include "../FileReader.h"
#include "../Unroller.h"
#include "libxml/HTMLparser.h"
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
	FileReader reader(config, start);
	int problem_count=unroller->get_problem_count();
	bool showUnsolved=true;
	//get from config file if unsolved problems should be displayed
	xmlNodePtr attr=(xmlNodePtr)config.findAttribute(start->properties,"HideUnsolved");
	if( attr!=NULL && !strcmp((char*)xmlNodeGetContent(attr),"Yes"))
		showUnsolved=false;

	if(reader.getType()!=FileReader::None)
	{
		//read standings into memory
		const char* buf=reader.read();
		if(buf)
		{

			int top=0;
			int len=strlen(buf);
			// skip till HTML tag
			for(int i=0;i<len;i++)
			{
				if(!strncmp(&buf[i],"<html>",6))
				{
					top=i;
					break;
				}
			}
			XMLParser xml((xmlDocPtr)htmlReadMemory(buf+top,len-top,NULL,NULL,0));
	// go down tree the following path
	//  1 / 2  /  3  / 4/ 5/  6   /  7  /8
	//html/body/table/tr/td/center/table/tr
			char html_path[][20]={
					"body",
					"table",
					"tr",
					"td",
					"center",
					"table",
					"tr"
			};
			xmlNodePtr start=xml.getRoot();
			for(int i=0;i<(int)(sizeof(html_path)/sizeof(html_path[0]));i++)
				start=xml.findNode(xml.getChild(start),html_path[i]);
			// skip first two lines, since they are just headers
			start=xml.findNode(xml.getNext(start),"tr");
			while(start)
			{
				start=xml.findNode(xml.getNext(start),"tr");
				if(!start) break;
				xmlNodePtr cur=xml.getChild(start);
				cur=xml.getNext(cur);
				if(!cur) break;
				// read information about team and store it as events
				string name=(char*)xmlNodeGetContent(cur);
				name=prefix+name;
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
			// sort all event
			sort(event.begin(),event.end());
			reverse(event.begin(),event.end());
		}
	}
}

NEERCHtmlParser::~NEERCHtmlParser() {
}

void NEERCHtmlParser::update()
{
}

void NEERCHtmlParser::updateContest(Contest* contest, int time)
{
	// process all events, which happen before time
	while( !event.empty() && event.back().time<=time)
	{
		pTeam team = contest->extract_team(event.back().team) ;
		// make all attempts and if accepted, mark as accepted
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
	return "Necessary attributes:\n"
		"<Path> - path to neerc standings\n"
#ifdef HAVE_LIBCURL
		" or\n"
		"<URL> - link to neerc standings\n"	
#endif
		"Optional attributes:\n"
		"<HideUnsolved> - [No/Yes] hide attempts on problems without AC";
}
