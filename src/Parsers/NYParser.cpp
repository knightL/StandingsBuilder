/*
 * NYParser.cpp
 *
 *      Author: knightl
 */

#include "../FileReader.h"
#include "../Unroller.h"
#include "libxml/HTMLparser.h"
#include "NYParser.h"
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <algorithm>
using namespace std;

NYParser::NYParser(const XMLParser& config, xmlNodePtr start):EventBasedParser(config,start) {
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
			xmlNodePtr start=xml.walkPath(xml.getRoot(), {">", "body", ">", "table", ">", "tbody", ">", "tr"});
			while(start)
			{
				start=xml.findNode(xml.getNext(start),"tr");
				if(!start) break;
				xmlNodePtr cur=xml.getChild(start);
				if(!cur) break;
				if(xml.findNode(cur, "th") != NULL) break;
				cur=xml.getNext(xml.findNode(cur,"td"));
				if(!cur) break;
				// read information about team and store it as events
				cur=xml.findNode(cur,"td");
				string name=xml.getNodeContent(cur);
				name=prefix+name;
				cur=xml.getNext(xml.findNode(cur,"td"));
				cur=xml.getNext(xml.findNode(cur,"td"));
				for(int i=0;i<problem_count;i++)
				{
					cur=xml.getNext(xml.findNode(cur,"td"));
					string t1=xml.getNodeContent(cur);
					for(int j=0;j<t1.size();j++)
						if(t1[j]=='/')
							t1[j]=' ';
					int acnt=-1;
					int tac=-1;
					sscanf(t1.c_str(), "%d%d",&acnt, &tac);
					if(tac==-1)
					{
						if(showUnsolved && acnt>0)
							this->add_event(0, name, i, false, acnt);
					}
					else
						this->add_event(tac, name, i, true, acnt-1);
				}
			}
		}
	}
}

NYParser::~NYParser() {
}

bool NYParser::providesTime()
{
	return false;
}

std::string NYParser::getName()
{
	return "NYParser";
}

std::string NYParser::getDescription()
{
	return "Necessary attributes:\n"
		"<Path> - path to new york standings\n"
#ifdef HAVE_LIBCURL
		" or\n"
		"<URL> - link to new york standings\n"	
#endif
		"Optional attributes:\n"
		"<HideUnsolved> - [No/Yes] hide attempts on problems without AC";
}
