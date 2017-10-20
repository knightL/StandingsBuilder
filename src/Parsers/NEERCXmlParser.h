/*
 * NEERCXmlParser.h
 *
 *      Author: knightl
 */

#ifndef NEERCXMLPARSER_H_
#define NEERCXMLPARSER_H_

#include "EventBasedParser.h"
#include "../XMLParser.h"
#include <string>

// Parses XML log in NEERC format
class NEERCXmlParser: public EventBasedParser {
public:
	NEERCXmlParser(const XMLParser& config, xmlNodePtr start);
	virtual ~NEERCXmlParser();
	virtual bool providesTime() ;
	static std::string getName();
	static std::string getDescription();
};

#endif /* NEERCXMLPARSER_H_ */
