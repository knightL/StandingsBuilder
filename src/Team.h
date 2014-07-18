/*
 * Team.h
 *
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
	/**
	 * if problem is solved, do nothing. if this is accepted submission, mark it solved and update penalty. if this it is not accepted, increase attempts count
	 **/
	void make_attempt(int t, int id, bool accepted);
	/**
	 * update all problem information and recalculate penalty and problem count
	 **/
	void make_from_description(std::vector<int> attempts, std::vector<bool> solved, std::vector<int> time);
	/**
	 * update problem information and results, in case it is impossible to get time submissions
	 **/
	void make_form_timeless_description(std::vector<int> attempts, std::vector<bool> solved, Result res);
	std::string get_name() const;
	/**
	 * sets team type. different type gives different color of background
	 **/
	void set_type(int type);
	Result get_result() const;

	friend class Contest;
};

#endif /* TEAM_H_ */
