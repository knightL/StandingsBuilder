/*
 * Unroller.h
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#ifndef UNROLLER_H_
#define UNROLLER_H_

#include "Parser.h"
#include "Contest.h"
#include "Event.h"
#include <vector>
#include <string>
#include <curl/curl.h>
#include "singletone.h"
#include "GenericParserBuilder.h"

class _Unroller {
	int problem_count;
	std::string output_file_name;
	int curtime;
	std::vector<GenericParserBuilder*> builders;
	std::vector<Parser*> parsers;
	int timeout;
public:
	Contest *contest;
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
};

typedef SingleTone<_Unroller> Unroller;

#endif /* UNROLLER_H_ */
