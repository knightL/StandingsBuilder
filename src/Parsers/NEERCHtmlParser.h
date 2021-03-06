/*
 * NEERCHtmlParser.h
 *
 *      Author: knightl
 */

#ifndef NEERCHTMLPARSER_H_
#define NEERCHTMLPARSER_H_

#include "EventBasedParser.h"

// Parses HTML standings from NEERC format
class NEERCHtmlParser: public EventBasedParser {
public:
	NEERCHtmlParser(const XMLParser& config, xmlNodePtr start);
	virtual ~NEERCHtmlParser();
	virtual bool providesTime() ;
	static std::string getName();
	static std::string getDescription();
};

#endif /* NEERCHTMLPARSER_H_ */
