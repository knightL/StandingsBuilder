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

NEERCHtmlParser::NEERCHtmlParser(const XMLParser& config, xmlNodePtr start):
	EventBasedParser(config,start) 
{
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
			xmlNodePtr start=xml.walkPath(xml.getRoot(),{">","body",">","table",">","tr",">","td",">","center",">","table",">","tbody",">","tr"});
			// skip first two lines, since they are just headers
			for(;start;start=xml.findNode(xml.getNext(start),"tr"))
			{
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
							this->add_event(0, name, i, false, -atoi(t1.c_str()));
					}
					else if(t1[0]=='+')
					{
						string t2=(char*)xmlNodeGetContent(xml.getChild(xml.getChild(cur)));
						t1=t1.substr(0, t1.size() - t2.size() );
						this->add_event( atoi(t2.c_str()), name, i, true, (t1.size()==1)?0:atoi(t1.c_str()) );
					}
				}
			}
		}
	}
}

NEERCHtmlParser::~NEERCHtmlParser() {
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
