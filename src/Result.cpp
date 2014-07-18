/*
 * Result.cpp
 *
 *      Author: knightl
 */

#include "Result.h"

Result::Result(int problems, int penalty):
	problems(problems),
	penalty(penalty)
{
}

bool Result::operator>(const Result& b) const
{
	if(problems != b.problems)
		return problems < b.problems;
	return penalty > b.penalty;
}

bool Result::operator==(const Result& b) const
{
	return problems == b.problems && penalty == b.penalty;
}

bool Result::operator<(const Result& b) const
{
	if(problems != b.problems)
		return problems > b.problems;
	return penalty < b.penalty;
}

bool Result::operator!=(const Result& b) const
{
	return problems != b.problems || penalty != b.penalty;
}

Result Result::operator+(const Result& b) const
{
	return Result(problems+b.problems, penalty+b.penalty);
}

Result& Result::operator+=(const Result& b)
{
	return (*this)=(*this)+b;
}
