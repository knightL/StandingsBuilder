/*
 * EventBasedParser.cpp
 *
 *      Author: knightl
 */

#include "../FileReader.h"
#include "../Unroller.h"
#include "libxml/HTMLparser.h"
#include "EventBasedParser.h"
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <algorithm>
using namespace std;

EventBasedParser::Event::Event(int time, std::string team, int id, bool accepted, int attempts):
	time(time),
	team(team),
	id(id),
	accepted(accepted),
	attempts(attempts)
{
}

bool EventBasedParser::Event::operator<(const Event& ev) const
{
	return time<ev.time;
}

EventBasedParser::EventBasedParser(const XMLParser& config, xmlNodePtr start):Parser(config,start) {
	
}

EventBasedParser::~EventBasedParser() {
}

void EventBasedParser::add_event(int time, std::string team_name, int problem_id, bool is_accepted, int failed_attempts)
{
	event.push(Event(time, team_name, problem_id, is_accepted, failed_attempts));
}

void EventBasedParser::update()
{
}

void EventBasedParser::updateContest(Contest* contest, int time)
{
	// process all events, which happen before time
	while( !event.empty() && event.top().time<=time)
	{
		pTeam team = contest->extract_team(event.top().team) ;
		// make all attempts and if accepted, mark as accepted
		for(int i=0;i<event.top().attempts;i++)
			team->make_attempt(event.top().time, event.top().id, false);
		if(event.top().accepted)
			team->make_attempt(event.top().time, event.top().id, true);
		team->set_type(style);
		contest->add_team(team);
		event.pop();
	}
}

