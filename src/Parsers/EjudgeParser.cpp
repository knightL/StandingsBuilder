/*
 * EjudgeParser.cpp
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#include "EjudgeParser.h"
#include "../Unroller.h"
#include <vector>
#include <cassert>
#include <cstring>
using namespace std;

EjudgeParser::EjudgeParser(const XMLParser& config, xmlNodePtr start):Parser(config,start) {
	xml=NULL;
	xmlNodePtr ptr=(xmlNodePtr)config.findAttribute(start->properties,"Timeless");
	if(!ptr)
		timeless=false;
	else
		timeless=!strcmp((char*)xmlNodeGetContent(ptr),"Yes");
}

EjudgeParser::~EjudgeParser() {
	if(xml)
		delete xml;
}

void EjudgeParser::updateContest(Contest* contest, int)
{
	if(!xml) return;
	Unroller unroller;
	int problem_count=unroller->get_problem_count();
	xmlNodePtr root=xml->getRoot();
	xmlNodePtr body=xml->findNode(xml->getChild(root), "body");
	xmlNodePtr state=xml->findNode(xml->getChild(body), "table");
	xmlNodePtr row=xml->getChild(state);
	vector<string> url_guys;
	vector<vector<int> > attempts;
	vector<vector<bool> > solved;
	vector<vector<int> > time;
	vector<Result> res;
	while(row)
	{
		row=xml->findNode(xml->getNext(row),"tr");
		if(!row) break;
		xmlNodePtr col=xml->getChild(row);
		if(atoi((char*)xmlNodeGetContent(col))==0) break;
		col=xml->getNext(col);
		url_guys.push_back((char*)xmlNodeGetContent(col));
		attempts.push_back(vector<int>(problem_count));
		solved.push_back(vector<bool>(problem_count));
		time.push_back(vector<int>(problem_count));
		for(int i=0;i<problem_count;i++)
		{
			col=xml->getNext(col);
			char buf[300];
			strcpy(buf,(char*)xmlNodeGetContent(col));
			for(int j=0;buf[j];j++)
				if(buf[j]=='('||buf[j]==')'||buf[j]==':')
					buf[j]=' ';
			for(int j=0;buf[j];j++)
			{
				if(buf[j]=='+')
				{
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
					solved.back()[i]=0;
					attempts.back()[i]=-atoi(&buf[j]);
					break;
				}
			}
		}
		if(timeless)
		{
			col=xml->getNext(col);
			int solved=atoi((char*)xmlNodeGetContent(col));
			col=xml->getNext(col);
			int penalty=atoi((char*)xmlNodeGetContent(col));
			res.push_back(Result(solved,penalty));
		}
	}
	for(int i=0;i<(int)this->teams.size();i++)
		delete unroller->contest->extract_team(teams[i]);
	this->teams=url_guys;
	for(int i=0;i<(int)this->teams.size();i++)
	{
		pTeam team=unroller->contest->extract_team(teams[i]);
		if(!timeless)
			team->make_from_description(attempts[i],solved[i],time[i]);
		else
			team->make_form_timeless_description(attempts[i],solved[i],res[i]);
		team->set_type(this->style);
		unroller->contest->add_team(team);
	}
	delete xml;
	xml=NULL;
}

bool EjudgeParser::providesTime()
{
	return true;
}
