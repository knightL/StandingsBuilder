/*
 * NEERCXmlParser.cpp
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#include "NEERCXmlParser.h"
#include <string>
#include <cassert>
#include <cstring>
#include <algorithm>
using namespace std;

NEERCXmlParser::Event::Event(int time, std::string team, int id, bool accepted):
	time(time),
	team(team),
	id(id),
	accepted(accepted)
{
}

bool NEERCXmlParser::Event::operator<(const Event& ev) const
{
	return time<ev.time;
}


NEERCXmlParser::NEERCXmlParser(const XMLParser& config, xmlNodePtr start):Parser(config,start) {
	start=(xmlNodePtr)config.findAttribute(start->properties,"XMLPath");
	if(!start)
		printf("NEERCXmlParser: Missing property \"XMLPath\"\n");
	else
	{
		string path=(char*)xmlNodeGetContent(start);
		xmlDoc* doc;
		xmlNode* root, *cur;
		doc = xmlReadFile(path.c_str(),NULL,0);
		if(doc==NULL)
		{
			printf("Failed to parse file %s\n",path.c_str());
			exit(1);
		}
		root=xmlDocGetRootElement(doc);
		assert(root!=NULL);
		cur=xmlFirstElementChild(root);
		assert(cur!=NULL);
		cur=xmlFirstElementChild(cur);
		for(;cur;cur=xmlNextElementSibling(cur))
		{
			if(!strcmp( (char*)cur->name, "session" ))
			{
				string name="";
				xmlAttrPtr attr=cur->properties;
				for(; attr; attr=attr->next)
					if( !strcmp( (char*)attr->name, "party") )
						name=(char*)xmlNodeGetContent((xmlNode*)attr);
				xmlNode* problems=xmlFirstElementChild(cur);
				for(;problems; problems= xmlNextElementSibling(problems))
				{
					attr=problems->properties;
					int id=-1;
					for(; attr; attr=attr->next)
						if( !strcmp( (char*)attr->name, "alias") )
						{
							xmlChar* buf=xmlNodeGetContent((xmlNode*)attr);
							id=buf[0]-'A';
						}
					assert(id>=0);
					xmlNode* runs=xmlFirstElementChild(problems);
					for(; runs; runs= xmlNextElementSibling(runs))
					{
						int accepted=-1;
						int time=-1;

						attr=runs->properties;
						for(; attr; attr=attr->next)
							if( !strcmp( (char*)attr->name, "accepted") )
							{
								xmlChar* buf=xmlNodeGetContent((xmlNode*)attr);
								if(buf[0]=='y')
									accepted=1;
								else
									accepted=0;
							}
							else if( !strcmp( (char*)attr->name, "time") )
							{
								xmlChar* buf=xmlNodeGetContent((xmlNode*)attr);
								time=atoi((char*)buf);
							}

						assert(accepted>=0);
						assert(time>=0);

						event.push_back(Event(time,name,id,accepted));
					}
				}
			}
		}
		xmlFreeDoc(doc);
		sort(event.begin(),event.end());
		reverse(event.begin(),event.end());
	}
}

NEERCXmlParser::~NEERCXmlParser() {
}

void NEERCXmlParser::update()
{
}

void NEERCXmlParser::updateContest(Contest* contest, int time)
{
	while( !event.empty() && event.back().time/1000/60<=time)
	{
		pTeam team = contest->extract_team(event.back().team) ;
		team->make_attempt(event.back().time/1000/60, event.back().id, event.back().accepted);
		team->set_type(style);
		contest->add_team(team);
		event.pop_back();
	}
}

bool NEERCXmlParser::providesTime()
{
	return false;
}

std::string NEERCXmlParser::getName()
{
	return "NEERCXmlParser";
}

std::string NEERCXmlParser::getDescription()
{
	return "Necessary attributes:\n\
<XMLPath> - path to xml file with neerc standings";
}
