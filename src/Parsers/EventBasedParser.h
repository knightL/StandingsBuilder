/*
 * EventBasedParser.h
 *
 *      Author: knightl
 */

#ifndef EVENTBASEDPARSER_H_
#define EVENTBASEDPARSER_H_

#include "../Parser.h"
#include <queue>

// Parses HTML standings from NEERC format
class EventBasedParser: public Parser {
	class Event {
		int time;
		std::string team;
		int id;
		bool accepted;
		int attempts;
	public:
		Event(int time, std::string team, int id, bool accepted, int attempts);
		bool operator<(const Event& ev) const;

		friend class EventBasedParser;
	};
	std::priority_queue<Event> event;
public:
	EventBasedParser(const XMLParser& config, xmlNodePtr start);
	virtual ~EventBasedParser();
	
	void add_event(int time, std::string team_name, int problem_id, bool is_accepted, int failed_attempts);
	
	virtual void update();
	virtual void updateContest(Contest* contest, int time);
};

#endif /* EVENTBASEDPARSER_H_ */
