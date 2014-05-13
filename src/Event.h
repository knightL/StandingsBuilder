/*
 * Event.h
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <string>

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

#endif /* EVENT_H_ */
