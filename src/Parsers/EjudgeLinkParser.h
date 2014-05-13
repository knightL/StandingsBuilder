/*
 * EjudgeLinkParser.h
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#ifndef EJUDGELINKPARSER_H_
#define EJUDGELINKPARSER_H_

#include "EjudgeParser.h"
#include <string>

class EjudgeLinkParser: public EjudgeParser {
	std::string url;
public:
	EjudgeLinkParser(const XMLParser& config, xmlNodePtr start);
	virtual ~EjudgeLinkParser();
	virtual void update();
	static std::string getName();
	static std::string getDescription();
};

#endif /* EJUDGELINKPARSER_H_ */
