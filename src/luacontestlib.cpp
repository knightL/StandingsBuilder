/*
 * Contest.h
 *
 *      Author: knightl
 */

#include "luacontestlib.h"
#include "Unroller.h"
#include "Contest.h"
int lua_count_problems(lua_State* lvm);
int lua_team_place(lua_State* lvm);
int lua_team_style(lua_State* lvm);
int lua_team_problems_solved(lua_State* lvm);
int lua_team_penalty(lua_State* lvm);
int lua_team_problem_attempts(lua_State* lvm);
int lua_team_is_solved(lua_State* lvm);
int lua_team_problem_time(lua_State* lvm);

static const luaL_Reg contestlib[]={
	{"countproblems"      , lua_count_problems},
	{"teamplace"          , lua_team_place},
	{"teamstyle"          , lua_team_style},
	{"teamproblemssolved" , lua_team_problems_solved},
	{"teampenalty"        , lua_team_penalty},
	{"teamproblemattempts", lua_team_problem_attempts},
	{"teamsolved"         , lua_team_solved},
	{"teamproblemtime"    , lua_team_problem_time},
	{NULL, NULL}	
};

void lua_contest_init(lua_State* lvm)
{
	luaL_newlib(lvm, contestlib);
	lua_setglobal(lvm, "contest");
}

int lua_count_problems(lua_State* lvm)
{
	lua_pushinteger(lvm, Unroller()->get_contest()->count_problems());
	return 1;
}

int lua_team_place(lua_State* lvm)
{
	const char* teamName=luaL_checkstring( lvm, 1 );
	lua_pushstring( lvm, Unroller()->get_contest()->get_team_place(teamName).c_str());
	return 1;
}

int lua_team_style(lua_State* lvm)
{
	const char* teamName=luaL_checkstring( lvm, 1 );
	lua_pushstring( lvm, Unroller()->get_contest()->get_team_style(teamName).c_str());
	return 1;
}

int lua_team_problems_solved(lua_State* lvm)
{
	const char* teamName=luaL_checkstring( lvm, 1 );
	lua_pushinteger( lvm, Unroller()->get_contest()->get_team_problems_solved(teamName));
	return 1;
}

int lua_team_penalty(lua_State* lvm)
{
	const char* teamName=luaL_checkstring( lvm, 1 );
	lua_pushinteger( lvm, Unroller()->get_contest()->get_team_penalty(teamName));
	return 1;
}

int lua_team_problem_attempts(lua_State* lvm)
{
	const char* teamName=luaL_checkstring( lvm, 1 );
	int pid=luaL_checkinteger( lvm, 2);
	lua_pushinteger( lvm, Unroller()->get_contest()->get_team_problem_attempts(teamName, pid-1));
	return 1;
}

int lua_team_solved(lua_State* lvm)
{
	const char* teamName=luaL_checkstring( lvm, 1 );
	int pid=luaL_checkinteger( lvm, 2);
	lua_pushboolean( lvm, Unroller()->get_contest()->get_team_solved(teamName, pid-1));
	return 1;
}

int lua_team_problem_time(lua_State* lvm)
{
	const char* teamName=luaL_checkstring( lvm, 1 );
	int pid=luaL_checkinteger( lvm, 2);
	lua_pushinteger( lvm, Unroller()->get_contest()->get_team_problem_time(teamName, pid-1));
	return 1;
}

