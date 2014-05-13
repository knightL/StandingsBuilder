/*
 * Contest.h
 *
 *  Created on: 25 апр. 2014
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
	pTeam extract_team(std::string name);
	void add_team(pTeam team);
	void print_standings(std::string file, int curtime);
};

#endif /* CONTEST_H_ */
