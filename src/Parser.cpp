/*
 * Parser.cpp
 *
 *      Author: knightl
 */

#include "Parser.h"
#include <cstdlib>

Parser::Parser(const XMLParser& config, xmlNodePtr start):curtime(0) {
	// if in XML config present information about style for parser, store it
	xmlNodePtr ptr=(xmlNodePtr)config.findAttribute(start->properties,"Style");
	if(!ptr)
		style=0;
	else
		style=atoi((char*)xmlNodeGetContent(ptr));
}

Parser::~Parser() {
}

int Parser::getTime()
{
	return curtime;
}

