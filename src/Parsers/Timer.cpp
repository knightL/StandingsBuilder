/*
 * Timer.cpp
 *
 *  Created on: 17 мая 2014
 *      Author: knightl
 */

#include "Timer.h"
#include <time.h>
#include <iostream>
using namespace std;

const int DAYMINUTES=24*60;

Timer::Timer(const XMLParser& config, xmlNodePtr start):Parser(config,start)
{
	daycount=0;
	start=(xmlNodePtr)config.findAttribute(start->properties,"StartTime");
	if(!start)
		printf("Timer: Missing property \"StartTime\"\n");
	else
	{
		string t=(char*)xmlNodeGetContent(start);
		int h=-1,m=-1;
		if(sscanf(t.c_str(),"%d:%d",&h,&m)!=2)
		{
			printf("Timer: Bad formated time\n");
		}
		starttime=h*60+m;
	}
}
Timer::~Timer()
{

}

void Timer::update()
{
	tm *p;
	time_t sec = time(NULL);

	if ((p = localtime(&sec)) != NULL)
	{
		int nxttime= p->tm_hour*60+ p->tm_min;
		curtime=nxttime-starttime;
		if(curtime<0) curtime+=DAYMINUTES;
	}
	else {
		printf("Failed to get system time\n");
	}
}
void Timer::updateContest(Contest* contest, int time) {}

bool Timer::providesTime()
{
	return true;
}

std::string Timer::getName()
{
	return "Timer";
}

std::string Timer::getDescription()
{
	return "Necessary attributes:\n\
<StartTime> - time, considered as start of contest";
}
