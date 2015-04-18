/*
 * NEERCHtmlParser.h
 *
 *      Author: knightl
 */

#ifndef NEERCHTMLPARSER_H_
#define NEERCHTMLPARSER_H_

#include "../Parser.h"

// Parses HTML standings from NEERC format
class NEERCHtmlParser: public Parser {
	class Event {
		int time;
		std::string team;
		int id;
		bool accepted;
		int attempts;
	public:
		Event(int time, std::string team, int id, bool accepted, int attempts);
		bool operator<(const Event& ev) const;

		friend class NEERCHtmlParser;
	};
	std::vector<Event> event;
public:
	NEERCHtmlParser(const XMLParser& config, xmlNodePtr start);
	virtual ~NEERCHtmlParser();
	virtual void update();
	virtual void updateContest(Contest* contest, int time);
	virtual bool providesTime() ;
	static std::string getName();
	static std::string getDescription();
};

#endif /* NEERCHTMLPARSER_H_ */
