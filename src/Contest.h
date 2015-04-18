/*
 * Contest.h
 *
 *      Author: knightl
 */

#ifndef CONTEST_H_
#define CONTEST_H_

#include <set>
#include <map>
#include "Team.h"
#include <lua.hpp>
#include "luacontestlib.h"

typedef std::set<std::pair<Result, pTeam> > Standings;

class Contest {
	Standings standings;
	int problem_count;
	std::map<std::string, pTeam> teams;
	std::map<std::string, std::string> team_place;
	lua_State *lvm;
public:
	Contest(int problem_cnt, std::string script_file_name);
	~Contest();

	/**
	 * Check if team is present
	 **/
	bool team_exists(std::string name) const;

	/**
	 * Find team by name, erase it from standings and return.
	 * If team is not found, new one will be created
	 **/
	pTeam extract_team(std::string name);

	/**
	 * Put team into standings. If there is, then the old one will be destroyed
	 * and replaced by new one
	 **/
	void add_team(pTeam team);

	/**
	 * Print standings in HTML format into file
	 **/
	void print_standings(std::string file, int curtime);

	/**
	 * Returns number of problems in contest
	 **/
	int count_problems() const;

	/**
	 * Computes teams places.
	 **/
	void prepare_teams_places();

	/**
	 * Returns team's place from the contest. Team place should be prepared first
	 **/
	std::string get_team_place(std::string name) const;
	
	/**
	 * Returns team's style
	 **/
	std::string get_team_style(std::string name) const;

	/**
	 * Returns number of solved problems by team
	 **/
	int get_team_problems_solved(std::string name) const;

	/**
	 * Return team's penalty
	 **/
	int get_team_penalty(std::string name) const;
	
	/**
	 * Return number of attempts of team on problem PID
	 **/
	int get_team_problem_attempts(std::string name, int pid) const;

	/**
	 * Return whether team solved problem PID
	 **/
	bool get_team_solved(std::string name, int pid) const;

	/**
	 * Return when was the problem PID solved by team
	 **/
	int get_team_problem_time(std::string name, int pid) const;
};

#endif /* CONTEST_H_ */
