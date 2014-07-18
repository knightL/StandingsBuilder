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

typedef std::set<std::pair<Result, pTeam> > Standings;

class Contest {
	Standings standings;
	int problem_count;
	std::map<std::string, pTeam> teams;
public:
	Contest(int problem_cnt);
	~Contest();
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
};

#endif /* CONTEST_H_ */
