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
#include <ctype.h>
#include <algorithm>
using namespace std;

NYParser::NYParser(const XMLParser& config, xmlNodePtr start):EventBasedParser(config,start) {
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
		//read standings into 
		const char* inp=reader.read();
		if(inp)
		{
			char *buf=(char*)calloc(strlen(inp)+1, sizeof(char));
			for(int i=0;inp[i];i++)
				buf[i]=inp[i];
			
			// tidy up the document before processing
			bool first_html=true;
			int top=0;
			int len=strlen(buf);
			
			// skip till HTML tag and remove multiple instances of it
			for(int i=0;i<len;i++)
			{
				const char* HTML="<html>";
				bool has_html=true;
				for(int j=0;j<6 && has_html;j++)
					has_html &= tolower(buf[i+j]) == HTML[j];
				if(has_html)
				{
					if(first_html)
					{
						top=i;
						first_html=false;
					}
					else
					{
						for(int j=0;j<6;j++)
							buf[i+j]=' ';
					}
				}
			}
			
			//delete uppercase tags <BODY>, <HEAD> and </BODY>, </HEAD>, </HTML> as they are duplicates of already existing
			char dup_tags[5][8]={"<BODY>", "<HEAD>", "</BODY>", "</HEAD>", "</HTML>"};
			int dup_len[5];
			for(int i=0;i<5;i++)
				dup_len[i]=strlen(dup_tags[i]);
			
			for(int i=0;i<len;i++)
			{
				for(int j=0;j<5;j++)
				{
					if( strncmp(&buf[i], dup_tags[j], dup_len[j]) == 0 )
						for(int k=0;k<dup_len[j];k++)
							buf[i+k]=' ';
				}
			}
			
			
			// process document and go to the first row. it should be skipped as it is the header
			XMLParser xml((xmlDocPtr)htmlReadMemory(buf+top,len-top,NULL,NULL,0));
			xmlNodePtr start=xml.walkPath(xml.getRoot(), {">", "body", ">", "table", ">", "tr"});
			while(start)
			{
				start=xml.findNode(xml.getNext(start),"tr");
				if(!start) break;
				xmlNodePtr cur=xml.getChild(start);
				if(!cur) break;
				if(xml.findNode(cur, "th") != NULL) break;
				// skip rank
				cur=xml.findNode(xml.getNext(xml.findNode(cur,"td")), "td");
				if(!cur) break;
				// read information about team and store it as events
				string name=xml.getNodeContent(cur);
				name=prefix+name;
				cur=xml.findNode(xml.getNext(cur),"td");
				cur=xml.findNode(xml.getNext(cur),"td");
				for(int i=0;i<problem_count;i++)
				{
					cur=xml.getNext(xml.findNode(cur,"td"));
					if(!cur)
					{
						fprintf(stderr,"NYParser: incomplete table row found\n");
						break;
					}
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
						this->add_event(tac*60, name, i, true, acnt-1);
				}
			}
			free(buf);
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
	return "New York Contest Parser\n"
	    "Necessary attributes:\n"
		"<Path> - path to new york standings\n"
#ifdef HAVE_LIBCURL
		" or\n"
		"<URL> - link to new york standings\n"	
#endif
		"Optional attributes:\n"
		"<HideUnsolved> - [No/Yes] hide attempts on problems without AC";
}
