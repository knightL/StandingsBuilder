/*
 * Timer.h
 *
 *  Created on: 17 мая 2014
 *      Author: knightl
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "../Parser.h"

class Timer: public Parser {
	int daycount;
	int starttime;
public:
	Timer(const XMLParser& config, xmlNodePtr start);
	virtual ~Timer();
	virtual void update();
	virtual void updateContest(Contest* contest, int time);
	virtual bool providesTime() ;
	static std::string getName();
	static std::string getDescription();
};

#endif /* TIMER_H_ */
