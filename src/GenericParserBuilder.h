/*
 * GenericParserBuilder.h
 *
 *  Created on: 13 мая 2014
 *      Author: knightl
 */

#ifndef GENERICPARSERBUILDER_H_
#define GENERICPARSERBUILDER_H_

#include "Parser.h"
#include <string>

class GenericParserBuilder {
public:
	GenericParserBuilder();
	virtual ~GenericParserBuilder();
	virtual Parser* build(const XMLParser& config, xmlNodePtr start)=0;
	virtual std::string getName() const=0;
	virtual std::string getDescription() const=0;
};

template<class ParserType>
class ParserBuilder:GenericParserBuilder {
public:
	ParserBuilder() {}
	virtual ~ParserBuilder() {}
	virtual Parser* build(const XMLParser& config, xmlNodePtr start)
	{
		return new ParserType(config,start);
	}
	virtual std::string getName() const
	{
		return ParserType::getName();
	}
	virtual std::string getDescription() const
	{
		return ParserType::getDescription();
	}
};

#endif /* GENERICPARSERBUILDER_H_ */
