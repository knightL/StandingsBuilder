/*
 * Parser.cpp
 *
 *      Author: knightl
 */

#include "Parser.h"
#include <cstdlib>

Parser::Parser(const XMLParser& config, xmlNodePtr start):curtime(0) {
	// if in XML config present information about style for parser, store it
	xmlAttrPtr ptr=config.findAttribute(start->properties,"Style");
	if(!ptr)
		style="";
	else
		style=config.getCurrentAttributeContent(ptr);
	ptr=config.findAttribute(start->properties,"Prefix");
	if(!ptr)
		prefix="";
	else
		prefix=config.getCurrentAttributeContent(ptr);
}

Parser::~Parser() {
}

int Parser::getTime()
{
	return curtime;
}

