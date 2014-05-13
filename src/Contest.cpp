/*
 * Contest.cpp
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#include "Contest.h"
#include <numeric>
#include <cassert>
#include <fstream>
#include <iomanip>
using namespace std;

Contest::Contest(int problem_cnt) {
	this->problem_count=problem_cnt;
}

Contest::~Contest()
{
	for(map<string,pTeam>::iterator it = this->teams.begin();
			it != this->teams.end() ; it++)
		delete it->second;
}


pTeam Contest::extract_team(string name)
{
	if(!this->teams.count(name))
	{
		pTeam team = new Team(name, this->problem_count);
		return team;
	}
	else
	{
		pTeam res=this->teams[name];
		this->teams.erase(this->teams.find(name));
		this->standings.erase(std::make_pair(res->get_result(),res));
		return res;
	}
}

void Contest::add_team(pTeam team)
{
	assert(this->teams.count(team->get_name())==0);
	this->teams[team->get_name()] = team;
	this->standings.insert(make_pair(team->get_result(),team));
}

void Contest::print_standings(std::string file, int curtime)
{
	ofstream out(file.c_str());
	if((out.rdstate()& std::ifstream::failbit)!=0)
	{
		printf("Failed to open file \"%s\"",file.c_str());
		return;
	}
	int hh,mm;
	hh=curtime/60;
	mm=curtime%60;
	out<<"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/><title>Положение участников</title></head><body>"<<endl<<"<h1>Положение участников ["<<hh<<":"<<setw(2)<<setfill('0')<<mm<<"]</h1>"<<endl;
	vector<pair<Result,pTeam> > stand(this->standings.begin(),this->standings.end());
	vector<int> total(problem_count);
	vector<int> success(problem_count);
	vector<int> first_success(problem_count,-1);
	for(int i=0;i<problem_count;i++)
		for(int j=0;j<(int)stand.size();j++)
		{
			total[i]+=stand[j].second->attempts[i] +(stand[j].second->solved[i]?1:0);
			success[i]+=stand[j].second->solved[i]?1:0;
			if(stand[j].second->solved[i] && stand[j].second->time[i]!=-1)
				if(first_success[i]==-1 || first_success[i]> stand[j].second->time[i])
					first_success[i]=stand[j].second->time[i];
		}
	int total_sum=accumulate(total.begin(),total.end(),0);
	int success_sum=accumulate(success.begin(),success.end(),0);
	out<<"<table border=\"1\">"<<endl;
	out<<"<tr>";
	out<<"<th>Место</th>";
	out<<"<th>Участник</th>";
	for(int i=0;i<problem_count;i++)
		out<<"<th>"<<(char)('A'+i)<<"</th>";
	out<<"<th>Всего</th>";
	out<<"<th>Штраф</th>";
	out<<"</tr>"<<endl;
	for(int i=0;i<(int)stand.size();)
	{
		int j;
		for(j=i+1; j<(int)stand.size() && stand[i].first==stand[j].first; j++);
		int spos=i+1;
		int epos=j;
		while(i<j)
		{
			pTeam cur=stand[i].second;
			if(cur->type==0)
				out<<"<tr>";
			else if(cur->type==1)
				out<<"<tr bgcolor=\"yellow\">";
			else if(cur->type==2)
				out<<"<tr bgcolor=\"orange\">";
			else if(cur->type==3)
				out<<"<tr bgcolor=\"pink\">";
			else if(cur->type==4)
				out<<"<tr bgcolor=\"silver\">";
			else
			{
				printf("Unknown style %d\n",cur->type);
				out<<"<tr>";
			}
			if(spos!=epos)
				out<<"<td>"<<spos<<"-"<<epos<<"</td>";
			else
				out<<"<td>"<<spos<<"</td>";
			out<<"<td>"<< cur->get_name() <<"</td>";

			for(int k=0;k<problem_count;k++)
			{
				if(!cur->solved[k] || cur->time[k]!=first_success[k] || cur->time[k]==-1)
					out<<"<td>";
				else
					out<<"<td bgcolor=\"lime\">";

				if(!cur->solved[k] && !cur->attempts[k])
					out<<"&nbsp;";
				else if(!cur->solved[k])
					out<<"-"<<cur->attempts[k]<<"";
				else if(cur->solved[k] && !cur->attempts[k])
					out<<"+";
				else
					out<<"+"<<cur->attempts[k]<<"";
				if(cur->solved[k])
				{
					if(cur->time[k]==-1)
						out<<" <div>(X:XX)</div>";
					else
					{
						int h=cur->time[k]/60;
						int m=cur->time[k]%60;
						out<<" <div>("<<h<<":"<<setw(2)<<setfill('0')<<m<<")</div>";
					}
				}
				out<<"</td>";
			}
			out<<"<td>"<<cur->get_result().problems<<"</td>";
			out<<"<td>"<<cur->get_result().penalty<<"</td>";
			out<<"</tr>"<<endl;
			i++;
		}
	}

	out<<"<tr><td>&nbsp;</td><td>Total:</td>";
	for(int i=0;i<problem_count;i++)
		out<<"<td>"<<total[i]<<"</td>";
	out<<"<td>"<<total_sum<<"</td><td>&nbsp;</td></tr>"<<endl;

	out<<"<tr><td>&nbsp;</td><td>Success:</td>";
	for(int i=0;i<problem_count;i++)
		out<<"<td>"<<success[i]<<"</td>";
	out<<"<td>"<<success_sum<<"</td><td>&nbsp;</td></tr>"<<endl;

	out<<"<tr><td>&nbsp;</td><td>%:</td>";
	for(int i=0;i<problem_count;i++)
		out<<"<td>"<<(total[i]==0?0:((success[i]*100+total[i]/2)/total[i]))<<"%</td>";
	out<<"<td>"<<(total_sum==0?0:(success_sum*100+total_sum/2)/total_sum)<<"%</td><td>&nbsp;</td></tr>"<<endl;

	out<<"<tr>";
	out<<"<th>Место</th>";
	out<<"<th>Участник</th>";
	for(int i=0;i<problem_count;i++)
		out<<"<th>"<<(char)('A'+i)<<"</th>";
	out<<"<th>Всего</th>";
	out<<"<th>Штраф</th>";
	out<<"</tr>"<<endl;

	out<<"</table></body></html>"<<endl;
	out.close();
}
