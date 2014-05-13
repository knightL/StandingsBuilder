/*
 * Event.cpp
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#include "Event.h"

Event::Event(int time, std::string team, int id, bool accepted):
	time(time),
	team(team),
	id(id),
	accepted(accepted)
{
}

bool Event::operator<(const Event& ev) const
{
	return time<ev.time;
}
