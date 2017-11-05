/*
 * Team.cpp
 *
 *      Author: knightl
 */

#include "Team.h"
#include <cassert>
#include <string>
using namespace std;

Team::Team(string name, int problem_cnt):
	name(name),
	result(0, 0),
	attempts(problem_cnt),
	solved(problem_cnt),
	time(problem_cnt)
{
	type="";
}

void Team::make_attempt(int t, int id, bool accepted)
{
	assert(0<=id && id<(int) this->solved.size());
	if(!this->solved[id])
	{
		if(accepted)
		{
			this->solved[id] = true;
			this->time[id]=t;
			this->result += Result(1,t/60 + this->attempts[id]*20);
		}
		else
			this->attempts[id]++;
	}
}

void Team::make_from_description(std::vector<int> attempts, std::vector<bool> solved, std::vector<int> time)
{
	this->attempts = attempts;
	this->solved = solved;
	this->time=time;
	this->result = Result(0, 0);
	for(int i=0 ; i<(int)attempts.size() ; i++)
		if(this->solved[i])
			this->result+=Result(1, time[i]/60 + this->attempts[i]*20 );
}

void Team::make_form_timeless_description(std::vector<int> attempts, std::vector<bool> solved, Result res)
{
	this->attempts = attempts;
	this->solved = solved;
	this->time=std::vector<int>(solved.size(),-1);
	this->result=res;
}

string Team::get_name() const
{
	return name;
}

Result Team::get_result() const
{
	return result;
}

void Team::set_type(std::string type)
{
	this->type=type;
}
