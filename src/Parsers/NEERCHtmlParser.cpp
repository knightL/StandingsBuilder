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
	xmlAttrPtr attr=config.findAttribute(start->properties,"HideUnsolved");
	if( attr!=NULL && config.getCurrentAttributeContent(attr)!="Yes")
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
			
			// Parse html
			XMLParser xml = XMLParser( (xmlDocPtr)htmlReadMemory(buf+top,len-top,NULL,NULL,0) );
			
			// Move to first row with team results
			xmlNodePtr start=xml.walkPath(xml.getRoot(),{">","body",">","table",">","tr",">","td",">","center",">","table",">","tbody",">","tr"});
			
			for(;start;start=xml.findNode(xml.getNext(start),"tr"))
			{
				// Enter the table row
				xmlNodePtr cur=xml.getChild(start);

				// Skip rank column
				cur=xml.getNext(cur);

				if(!cur)
				{
					fprintf(stderr, "NEERCHtmlParser: Row without team name encountered\n");
					break;
				}
				
				// read information about team and store it as events
				string name=prefix + xml.getNodeContent(cur);

				for(int i=0;i<problem_count;i++)
				{
					cur=xml.getNext(cur);
					string t1=xml.getNodeContent(cur);
					
					// parse content of Ith problem contents
					if(t1[0]=='-')
					{
						if(showUnsolved)
							this->add_event(0, name, i, false, -atoi(t1.c_str()));
					}
					else if(t1[0]=='+')
					{
						//get content of div tag
						string t2=xml.getNodeContent(xml.getChild(xml.getChild(cur)));
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
