/*
 * EjudgeParser.cpp
 *
 *      Author: knightl
 */

#include "EjudgeParser.h"
#include "../Unroller.h"
#include "../FileReader.h"
#include "libxml/HTMLparser.h"
#include <vector>
#include <sstream>
#include <cassert>
#include <cstring>
#include <algorithm>
using namespace std;

EjudgeParser::EjudgeParser(const XMLParser& config, xmlNodePtr start):
	Parser(config,start), 
	FileReader(config,start)
{
	xml=NULL;
	// get from config, if parser should get time of submissions
	xmlNodePtr ptr=(xmlNodePtr)config.findAttribute(start->properties,"Timeless");
	if(!ptr)
		timeless=false;
	else
		timeless=!strcmp((char*)xmlNodeGetContent(ptr),"Yes");
	ptr=(xmlNodePtr)config.findAttribute(start->properties,"IgnoreColumns");
	if(ptr!=NULL)
	{
		istringstream in((char*)xmlNodeGetContent(ptr));
		int id;
		while(in>>id)
			ignore.push_back(id-1);
		sort(ignore.begin(), ignore.end());
		ignore.erase( unique(ignore.begin(), ignore.end()), ignore.end());
	}
}

EjudgeParser::~EjudgeParser() {
	if(xml)
		delete xml;
}

void EjudgeParser::update()
{
	if(xml) delete xml;
	const char *HTMLbuf = read();
	if(HTMLbuf)
	{
		int top=0;
		int len=strlen(HTMLbuf);
		// skip till HTML tag
		for(int i=0;i<len;i++)
		{
			if(!strncmp(&HTMLbuf[i],"<html>",6))
			{
				top=i;
				break;
			}
		}
		xml = new XMLParser((xmlDocPtr)htmlReadMemory(HTMLbuf+top,len-top,NULL,NULL,0));

		// read time from XML tree
		xmlNodePtr root=xml->getRoot();
		xmlNodePtr body=xml->findNode(xml->getChild(root), "body");
		xmlNodePtr state=xml->findNode(xml->getChild(body), "h1");
		if(state)
		{
			char* description= (char*)xmlNodeGetContent(state);
			int i;
			for(i=0;description[i];i++)
				if(description[i]=='[')
				{
					i++;
					break;
				}
			int h,m,s;
			if(sscanf(description+i,"%d:%d:%d",&h,&m,&s)==3)
				curtime=m+60*h;
			else
				printf("Failed to read time");
		}
	}
}

void EjudgeParser::skip_ignored(xmlNodePtr &col, int &ignore_top, int &col_id) const
{
	while (true)
	{
		while(ignore_top<(int)ignore.size() && ignore[ignore_top]<col_id )
		{
			ignore_top++;
		}
	
		if( ignore_top<(int)ignore.size() && ignore[ignore_top]==col_id )
		{
			col=xml->getNext(col);
			col_id++;
		}
		else
			break;
	}
}

void EjudgeParser::updateContest(Contest* contest, int)
{
	if(!xml) return;
	Unroller unroller;
	int problem_count=unroller->get_problem_count();
	// get down in XML tree to rows of table
	xmlNodePtr root=xml->getRoot();
	xmlNodePtr body=xml->findNode(xml->getChild(root), "body");
	xmlNodePtr state=xml->findNode(xml->getChild(body), "table");
	xmlNodePtr row=xml->getChild(state);

	vector<string> url_guys;
	vector<vector<int> > attempts;
	vector<vector<bool> > solved;
	vector<vector<int> > time;
	vector<Result> res;
	// parse every row of the table
	while(row)
	{
		int ignore_top=0;
		int col_id=0;
		
		row=xml->findNode(xml->getNext(row),"tr");
		if(!row) break;
		
		xmlNodePtr col=xml->getChild(row);
		skip_ignored(col, ignore_top, col_id);
		
		// if we can't get place from first column, then we are done with teams
		if(atoi((char*)xmlNodeGetContent(col))==0) break;
		
		col=xml->getNext(col);
		col_id++;
		skip_ignored(col, ignore_top, col_id);
		
		// get team name and allocate place to store information about submission
		url_guys.push_back(prefix+string((char*)xmlNodeGetContent(col)));
		attempts.push_back(vector<int>(problem_count));
		solved.push_back(vector<bool>(problem_count));
		time.push_back(vector<int>(problem_count));
		for(int i=0;i<problem_count;i++)
		{
			col=xml->getNext(col);
			col_id++;
			skip_ignored(col, ignore_top, col_id);
			
			char buf[300];
			// wipe out all punctuation parentheses and colons
			strcpy(buf,(char*)xmlNodeGetContent(col));
			for(int j=0;buf[j];j++)
				if(buf[j]=='('||buf[j]==')'||buf[j]==':')
					buf[j]=' ';
			// find first + or - if any
			for(int j=0;buf[j];j++)
			{
				if(buf[j]=='+')
				{
					// set problem solved, and store number of attempts and accept times
					solved.back()[i]=1;
					j++;
					if(!isdigit(buf[j]))
						attempts.back()[i]=0;
					else
						attempts.back()[i]=atoi(&buf[j]);
					int h=0,m=-1;
					if(!timeless)
						assert(sscanf(buf,"%*s%d%d",&h,&m)==2);
					time.back()[i]=60*h+m;
					break;
				}
				else if(buf[j]=='-')
				{
					// store number of attempts
					solved.back()[i]=0;
					attempts.back()[i]=-atoi(&buf[j]);
					break;
				}
			}
		}
		if(timeless)
		{
			// if we can't get time of submission lets store number of solved problems and penalty
			col=xml->getNext(col);
			col_id++;
			skip_ignored(col, ignore_top, col_id);
			
			int solved=atoi((char*)xmlNodeGetContent(col));
			col=xml->getNext(col);
			col_id++;
			skip_ignored(col, ignore_top, col_id);
			
			int penalty=atoi((char*)xmlNodeGetContent(col));
			res.push_back(Result(solved,penalty));
		}
	}
	// remove all teams, that were added at previous step
	for(int i=0;i<(int)this->teams.size();i++)
		delete contest->extract_team(teams[i]);
	// put new teams into table
	this->teams=url_guys;
	for(int i=0;i<(int)this->teams.size();i++)
	{
		pTeam team=contest->extract_team(teams[i]);
		if(!timeless)
			team->make_from_description(attempts[i],solved[i],time[i]);
		else
			team->make_form_timeless_description(attempts[i],solved[i],res[i]);
		team->set_type(this->style);
		contest->add_team(team);
	}
	delete xml;
	xml=NULL;
}

bool EjudgeParser::providesTime()
{

	return true;
}

std::string EjudgeParser::getName()
{
	return "EjudgeParser";
}

std::string EjudgeParser::getDescription()
{
	return "Necessary attributes:\n"
		   "<Path> - path to ejudge standings\n"
#ifdef HAVE_LIBCURL
			" or\n"
			"<URL> - url to ejudge standings\n"
#endif
           "Optional attributes:\n"
		   "<Timeless> - [No/Yes] whether standings have time of submission or not\n"
		   "<IgnoreColumns> - Space separated list of column numbers that will not be processed";

}

