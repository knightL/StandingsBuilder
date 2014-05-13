/*
 * Team.h
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#ifndef TEAM_H_
#define TEAM_H_

#include <string>
#include <vector>
#include "Result.h"

typedef class Team* pTeam;

class Team {
	std::string name;
	int type;
	Result result;
	std::vector<int> attempts;
	std::vector<bool> solved;
	std::vector<int> time;
public:
	Team(std::string name, int problem_cnt);
	void make_attempt(int t, int id, bool accepted);
	void make_from_description(std::vector<int> attempts, std::vector<bool> solved, std::vector<int> time);
	void make_form_timeless_description(std::vector<int> attempts, std::vector<bool> solved, Result res);
	std::string get_name() const;
	void set_type(int type);
	Result get_result() const;
	friend class Contest;
};

#endif /* TEAM_H_ */
