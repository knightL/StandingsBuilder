/*
 * EjudgeParser.h
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#ifndef EJUDGEPARSER_H_
#define EJUDGEPARSER_H_

#include "../Parser.h"
#include "../XMLParser.h"

class EjudgeParser: public Parser {
protected:
	XMLParser *xml;
	std::vector<std::string> teams;
	bool timeless;
public:
	EjudgeParser(const XMLParser& config, xmlNodePtr start);
	virtual ~EjudgeParser();
	virtual void updateContest(Contest* contest, int time);
	virtual bool providesTime();
};

#endif /* EJUDGEPARSER_H_ */
