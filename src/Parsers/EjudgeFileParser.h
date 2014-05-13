/*
 * EjudgeFileParser.h
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#ifndef EJUDGEFILEPARSER_H_
#define EJUDGEFILEPARSER_H_

#include "EjudgeParser.h"
#include <string>

class EjudgeFileParser: public EjudgeParser {
	std::string path;
public:
	EjudgeFileParser(const XMLParser& config, xmlNodePtr start);
	virtual ~EjudgeFileParser();
	virtual void update();
	static std::string getName();
	static std::string getDescription();
};

#endif /* EJUDGEFILEPARSER_H_ */
