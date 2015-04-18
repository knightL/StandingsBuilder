/*
 * Contest.h
 *
 *      Author: knightl
 */

#ifndef LUACONTESTLIB_H_
#define LUACONTESTLIB_H_

#include <lua.hpp>

void lua_contest_init(lua_State* lvm);
int lua_count_problems(lua_State* lvm);
int lua_team_place(lua_State* lvm);
int lua_team_style(lua_State* lvm);
int lua_team_place(lua_State* lvm);
int lua_team_problems_solved(lua_State* lvm);
int lua_team_penalty(lua_State* lvm);
int lua_team_problem_attempts(lua_State* lvm);
int lua_team_solved(lua_State* lvm);
int lua_team_problem_time(lua_State* lvm);


#endif /* LUACONTESTLIB_H_ */
