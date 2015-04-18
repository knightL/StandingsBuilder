/*
 * Unroller.h
 *
 *      Author: knightl
 */

#ifndef UNROLLER_H_
#define UNROLLER_H_

#include "Parser.h"
#include "Contest.h"
#include <vector>
#include <string>
#include "singletone.h"
#include "GenericParserBuilder.h"

class _Unroller {
	int problem_count;
	std::string output_file_name;
	int curtime;
	std::vector<GenericParserBuilder*> builders;
	std::vector<Parser*> parsers;
	int refresh_rate;
	Contest *contest;
public:
	_Unroller();
	~_Unroller();
	void run();
	void init(int argc, char** argv);
	template<class ParserType> void registerParser()
	{
		this->builders.push_back((GenericParserBuilder*)new ParserBuilder<ParserType>());
	}
	void print_help(std::string name);
	int get_problem_count() const;
	Contest* get_contest();
};

typedef SingleTone<_Unroller> Unroller;

#endif /* UNROLLER_H_ */
