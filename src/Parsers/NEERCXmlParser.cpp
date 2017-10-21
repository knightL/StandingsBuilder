/*
 * NEERCXmlParser.cpp
 *
 *      Author: knightl
 */

#include "NEERCXmlParser.h"
#include "../FileReader.h"
#include <string>
#include <cassert>
#include <cstring>
#include <algorithm>
using namespace std;

NEERCXmlParser::NEERCXmlParser(const XMLParser& config, xmlNodePtr start): 
	EventBasedParser(config,start) 
{
	FileReader reader(config, start);
	
	if(reader.getType()!=FileReader::None)
	{
		const char* buf=reader.read();
		if(buf)
		{
			xmlNodePtr root, cur;
			
			XMLParser xml = XMLParser( xmlReadMemory(buf, strlen(buf), NULL, NULL, 0) );

			// go down XML tree
			root=xml.getRoot();

			cur=xml.getChild(xml.getChild(root));
			for(;cur;cur=xml.findNode(xml.getNext(cur), "session"))
			{
				// get team name
				string name=prefix + xml.getAttributeContent(cur,"party");
				// for each problem
				xmlNodePtr problems=xml.getChild(cur);
				for(;problems; problems= xml.getNext(problems))
				{
					// get problem id
					string sid=xml.getAttributeContent(problems, "alias");
					if(sid.empty() || sid[0]<'A' || sid[0]>'Z')
					{
						fprintf(stderr, "NEERCXmlParser: Bad formed XML: no alias for a problem found\n");
						continue;
					}
					int id=sid[0]-'A';
					
					// store all submission
					xmlNodePtr runs=xml.getChild(problems);
					for(; runs; runs= xml.getNext(runs))
					{
						int accepted= xml.getAttributeContent(runs, "accepted")=="yes";
						int time=atoi(xml.getAttributeContent(runs, "time").c_str())/1000/60;

						this->add_event(time, name, id, accepted, accepted? 0:1 );
					}
				}
			}
		}
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
