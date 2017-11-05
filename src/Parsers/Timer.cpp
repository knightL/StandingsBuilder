/*
 * Timer.cpp
 *
 *      Author: knightl
 */

#include "Timer.h"
#include <time.h>
#include <iostream>
#include <string>
using namespace std;

Timer::Timer(const XMLParser& config, xmlNodePtr start):Parser(config,start)
{
	// read starting time
	int y=0,m=0,d=0;
	int hh=0,mm=0;
	tm t;
	xmlAttrPtr ptr=config.findAttribute(start->properties,"StartTime");
	if(!ptr)
		printf("Timer: Missing property \"StartTime\"\n");
	else
	{
		string t=config.getCurrentAttributeContent(ptr);
		if(sscanf(t.c_str(),"%d:%d",&hh,&mm)!=2)
		{
			printf("Timer: Bad formated time\n");
		}
	}
	ptr=config.findAttribute(start->properties,"StartDate");
	if(!ptr)
	{
		tm *p;
		time_t sec = time(NULL);
		if ((p = localtime(&sec)) != NULL)
		{
			y=p->tm_year;
			m=p->tm_mon;
			d=p->tm_mday;
		}
		else
		{
			printf("Failed to get system time\n");
		}
	}
	else
	{
		string t=config.getCurrentAttributeContent(ptr);
		int v=sscanf(t.c_str(),"%d/%d/%d",&y,&m,&d);
		if(v!=3)
			printf("Timer: Bad formated date\n");
		if(v>=1)
			y-=1900;
		if(v>=2)
			m--;
	}
	tm curt;
    curt.tm_year=y;
    curt.tm_mon=m;
    curt.tm_mday=d;
    curt.tm_hour=hh;
    curt.tm_min=mm;
    curt.tm_sec=0;
	curt.tm_isdst=-1;
	starttime=mktime(&curt);
}
Timer::~Timer()
{

}

void Timer::update()
{
	time_t nxttime=time(0);
	
	curtime=nxttime-starttime;
	// if nxttime < starttime then contest hasn't started
	if(curtime<0)
		curtime=0;
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
	return "Necessary attributes:\n"
"<StartTime> - time, considered as start of contest\n"
"Optional attributes:\n"
"<StartDate> - date, considered as start of contest. Format yyyy/mm/dd";
}
