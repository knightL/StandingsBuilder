/*
 * EjudgeParser.h
 *
 *      Author: knightl
 */

#ifndef EJUDGEPARSER_H_
#define EJUDGEPARSER_H_

#include "../Parser.h"
#include "../XMLParser.h"
#include "../FileReader.h"

class EjudgeParser: public Parser, public FileReader {
protected:
	XMLParser *xml;
	std::vector<std::string> teams;
	std::vector<int> ignore;
	bool timeless;
	void skip_ignored(xmlNodePtr &cur, int &ignore_top, int &col_id) const;
public:
	EjudgeParser(const XMLParser& config, xmlNodePtr start);
	virtual ~EjudgeParser();
	virtual void update();
	virtual void updateContest(Contest* contest, int time);
	virtual bool providesTime();
	static std::string getName();
	static std::string getDescription();
};

#endif /* EJUDGEPARSER_H_ */
