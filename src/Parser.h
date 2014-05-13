/*
 * Parser.h
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Contest.h"
#include "XMLParser.h"

class Parser {
protected:
	int curtime;
	int style;
public:
	Parser(const XMLParser& config, xmlNodePtr start);
	virtual ~Parser();
	virtual void update()=0;
	virtual void updateContest(Contest* contest, int time)=0;
	virtual bool providesTime()=0;
	int getTime();
};

#endif /* PARSER_H_ */
