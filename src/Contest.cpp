/*
 * Contest.cpp
 *
 *      Author: knightl
 */

#include "Contest.h"
#include <numeric>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
using namespace std;

Contest::Contest(int problem_cnt, std::string script_file_name, const XMLParser& config) {
	this->problem_count=problem_cnt;

	//initalize Lua
	this->lvm=luaL_newstate();
	if( lvm == NULL)
	{
		printf("Failed to initialize Lua\n");
		exit(2);
	}

	luaL_openlibs( lvm );
	//initilize contest library for Lua
	lua_contest_init(lvm);

	//run script from file
	if ( luaL_dofile( lvm, script_file_name.c_str() ) )
	{
		printf("Failed to load LUA script from file \"%s\"\n%s\n",script_file_name.c_str(), lua_tostring( lvm, -1) );
		exit(2);
	}
	//check, that function printStandings is present
	lua_getglobal( lvm, "printStandings" );
	if ( !lua_isfunction( lvm, -1) )
	{
		printf("No printStandings function in LUA script\n");
		exit(2);
	}
	
	//clear stack
	lua_pop( lvm, 1 );
	
	//check if init function is present and call it
	lua_getglobal( lvm, "init" );

	if ( lua_isfunction( lvm, -1) )
	{
		lua_newtable( lvm );
		xmlNodePtr node = config.findNode(config.getChild(config.getRoot()),"OutputScript");
		for(xmlAttrPtr attr = config.getAttributeList(node); attr != NULL; attr=config.getNextAttribute(attr))
		{
			lua_pushstring( lvm, (char*)attr->name);
			lua_pushstring( lvm, config.getCurrentAttributeContent(attr).c_str());
			lua_settable( lvm, -3 );
		}
		lua_call( lvm, 1, 0);
	}
	else
		lua_pop( lvm, 1 );
}

Contest::~Contest()
{
	// delete all allocated teams
	for(map<string,pTeam>::iterator it = this->teams.begin();
			it != this->teams.end() ; it++)
		delete it->second;
	// close Lua
	lua_close( lvm );
}

bool Contest::team_exists(string name) const
{
	return this->teams.count(name);
}

pTeam Contest::extract_team(string name)
{
	if(!team_exists(name))
	{
		// if there is no team with such name, then allocate new
		pTeam team = new Team(name, this->problem_count);
		return team;
	}
	else
	{
		//remove team from standings and return it as rresult
		pTeam res=this->teams[name];
		this->teams.erase(this->teams.find(name));
		this->standings.erase(std::make_pair(res->get_result(),res));
		return res;
	}
}

void Contest::add_team(pTeam team)
{
	if( this->teams.count(team->get_name())==1 )
	{
		// if there already exists team with same name, then delete it
		pTeam tmp= this->teams[ team->get_name() ];
		this->teams.erase( tmp->get_name() );
		this->standings.erase(std::make_pair(tmp->get_result(),tmp));
		delete tmp;
	}

	this->teams[team->get_name()] = team;
	this->standings.insert(make_pair(team->get_result(),team));
}


void Contest::print_standings(std::string file, int curtime)
{
	prepare_teams_places();

	// open output file
	lua_getglobal( lvm, "io");
	lua_pushstring(lvm, "open");
	lua_gettable(lvm, -2);
	lua_pushstring(lvm, file.c_str());
	lua_pushstring(lvm, (const char*) "w");
	lua_call(lvm, 2, 2);
	if(lua_isstring (lvm, -1) )
	{
		printf("Failed to open file \"%s\"\n%s\n",file.c_str(), lua_tostring(lvm, -1));
		lua_pop( lvm, 2);
		return;
	}
	lua_pop(lvm,1);
	
	//place printStandings to stack
	lua_getglobal( lvm, "printStandings" );

	//push file as first argument
	lua_pushvalue( lvm, -2);

	//create and fill table of users ordered by their place
	vector<pair<Result,pTeam> > stand(this->standings.begin(),this->standings.end());
	lua_createtable ( lvm, stand.size(), 0);
	for(int i=0;i<stand.size();i++)
	{
		lua_pushinteger( lvm, i+1);
		lua_pushstring( lvm, stand[i].second->get_name().c_str() );
		lua_settable( lvm, -3 );
	}

	// push current time as third argument
	lua_pushinteger(lvm, curtime);
	// call function
	lua_call( lvm, 3, 0);
	
	// close file
	lua_pushstring(lvm, "close");
	lua_gettable(lvm, -3);
	lua_insert(lvm, -2);
	lua_call(lvm, 1, 0);

	// clear stack
	lua_pop(lvm, 1);
}

int Contest::count_problems() const
{
	return problem_count;
}

void Contest::prepare_teams_places()
{
	team_place.clear();
	vector<pair<Result,pTeam> > stand(this->standings.begin(),this->standings.end());
	for(int i=0;i<(int)stand.size();)
	{
		int j;
		// find last team, whitch equals to i-th
		for(j=i+1; j<(int)stand.size() && stand[i].first==stand[j].first; j++);
		int spos=i+1;
		int epos=j;
		ostringstream s;
		s<<spos;
		if(epos!=spos) s<<"-"<<epos;
		string pos=s.str();
		for(;i<j;i++)
			team_place[stand[i].second->get_name()]=pos;
	}
}

string Contest::get_team_place(string name) const
{
	map<string, string>::const_iterator it=team_place.find(name);
	if(it==team_place.end()) return "";
	else return it->second;
}

string Contest::get_team_style(string name) const
{
	map<string, pTeam>::const_iterator it=teams.find(name);
	if(it==teams.end()) return "";
	else return it->second->type;
}

int Contest::get_team_problems_solved(string name) const
{
	map<string, pTeam>::const_iterator it=teams.find(name);
	if(it==teams.end()) return 0;
	else return it->second->result.problems;
}

int Contest::get_team_penalty(string name) const
{
	map<string, pTeam>::const_iterator it=teams.find(name);
	if(it==teams.end()) return 0;
	else return it->second->result.penalty;
}

int Contest::get_team_problem_attempts(string name, int pid) const
{
	if(0<=pid && pid<problem_count)
	{
		map<string, pTeam>::const_iterator it=teams.find(name);
		if(it==teams.end()) return 0;
		else return it->second->attempts[pid];
	}
	else
		return 0;
}

bool Contest::get_team_solved(string name, int pid) const
{
	if(0<=pid && pid<problem_count)
	{
		map<string, pTeam>::const_iterator it=teams.find(name);
		if(it==teams.end()) return false;
		else return it->second->solved[pid];
	}
	else
		return false;
}

int Contest::get_team_problem_time(string name, int pid) const
{
	if(0<=pid && pid<problem_count)
	{
		map<string, pTeam>::const_iterator it=teams.find(name);
		if(it==teams.end()) return -1;
		else return it->second->time[pid];
	}
	else
		return -1;
}

