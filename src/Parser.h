/*
 * Parser.h
 *
 *      Author: knightl
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string>
#include "Contest.h"
#include "XMLParser.h"

class Parser {
protected:
	int curtime;
	std::string style;
	std::string prefix;
public:
	Parser(const XMLParser& config, xmlNodePtr start);
	virtual ~Parser();
	/**
	 * Update parser information
	 **/
	virtual void update()=0;
	/**
	 * Update contest with parser information
	 **/
	virtual void updateContest(Contest* contest, int time)=0;
	/**
	 * Returns if parser can be used to get time from it
	 **/
	virtual bool providesTime()=0;
	int getTime();
	// also must have following static functions for builder to work correctly:
	// static std::string getName();
	// static std::string getDescription();
};

#endif /* PARSER_H_ */
