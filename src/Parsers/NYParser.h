/*
 * NYParser.h
 *
 *      Author: knightl
 */

#ifndef NYPARSER_H_
#define NYPARSER_H_

#include "EventBasedParser.h"

// Parses HTML standings from NEERC format
class NYParser: public EventBasedParser {
public:
	NYParser(const XMLParser& config, xmlNodePtr start);
	virtual ~NYParser();
	virtual bool providesTime() ;
	static std::string getName();
	static std::string getDescription();
};

#endif /* NEERCHTMLPARSER_H_ */
