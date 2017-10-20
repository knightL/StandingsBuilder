/*
 * NEERCXmlParser.cpp
 *
 *      Author: knightl
 */

#include "NEERCXmlParser.h"
#include <string>
#include <cassert>
#include <cstring>
#include <algorithm>
using namespace std;

NEERCXmlParser::NEERCXmlParser(const XMLParser& config, xmlNodePtr start): 
	EventBasedParser(config,start) 
{
	// Get XML file path from config and open it

	start=(xmlNodePtr)config.findAttribute(start->properties,"Path");
	if(!start)
		printf("NEERCXmlParser: Missing property \"Path\"\n");
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
		// go down XML tree
		root=xmlDocGetRootElement(doc);
		assert(root!=NULL);
		cur=xmlFirstElementChild(root);
		assert(cur!=NULL);
		cur=xmlFirstElementChild(cur);
		for(;cur;cur=xmlNextElementSibling(cur))
		{
			// for each team
			if(!strcmp( (char*)cur->name, "session" ))
			{
				string name="";
				// find team's name
				xmlAttrPtr attr=cur->properties;
				for(; attr; attr=attr->next)
					if( !strcmp( (char*)attr->name, "party") )
						name=prefix+string((char*)xmlNodeGetContent((xmlNode*)attr));
				// for each problem
				xmlNode* problems=xmlFirstElementChild(cur);
				for(;problems; problems= xmlNextElementSibling(problems))
				{
					// get problem id
					attr=problems->properties;
					int id=-1;
					for(; attr; attr=attr->next)
						if( !strcmp( (char*)attr->name, "alias") )
						{
							xmlChar* buf=xmlNodeGetContent((xmlNode*)attr);
							id=buf[0]-'A';
						}
					assert(id>=0);
					// store all submission
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

						this->add_event(time, name, id, accepted, accepted? 0:1 );
					}
				}
			}
		}
		xmlFreeDoc(doc);
	}
}

NEERCXmlParser::~NEERCXmlParser() {
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
	return "Necessary attributes:\n"
		   "<Path> - path to xml file with neerc standings"
#ifdef HAVE_LIBCURL
		   "\n or\n"
		   "<URL> - link to xml file"
#endif 
;
}
