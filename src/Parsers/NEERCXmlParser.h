/*
 * NEERCXmlParser.h
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#ifndef NEERCXMLPARSER_H_
#define NEERCXMLPARSER_H_

#include "../Parser.h"
#include "../XMLParser.h"
#include <string>

class NEERCXmlParser: public Parser {
	class Event {
		int time;
		std::string team;
		int id;
		bool accepted;
	public:
		Event(int time, std::string team, int id, bool accepted);
		bool operator<(const Event& ev) const;

		friend class NEERCXmlParser;
	};
	std::vector<Event> event;
public:
	NEERCXmlParser(const XMLParser& config, xmlNodePtr start);
	virtual ~NEERCXmlParser();
	virtual void update();
	virtual void updateContest(Contest* contest, int time);
	virtual bool providesTime() ;
	static std::string getName();
	static std::string getDescription();
};

#endif /* NEERCXMLPARSER_H_ */
