/*
 * Timer.h
 *
 *      Author: knightl
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "../Parser.h"
#include <ctime>

// Doesn't change contest standings, but only counts minutes since contest start
class Timer: public Parser {
	time_t starttime;
public:
	Timer(const XMLParser& config, xmlNodePtr start);
	virtual ~Timer();
	virtual void update();
	virtual void updateContest(Contest* contest, int time);
	virtual bool providesTime() ;
	int getCurrentTime();
	static std::string getName();
	static std::string getDescription();
};

#endif /* TIMER_H_ */
