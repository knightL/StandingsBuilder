/*
 * Result.h
 *
 *      Author: knightl
 */

#ifndef RESULT_H_
#define RESULT_H_

class Result {
public:
	int problems, penalty;
	Result(int problems, int penalty);
	bool operator<(const Result& b) const;
	bool operator==(const Result& b) const;
	bool operator>(const Result& b) const;
	bool operator!=(const Result& b) const;
	Result operator+(const Result& b) const;
	Result& operator+=(const Result& b);
};

#endif /* RESULT_H_ */
