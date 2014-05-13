/*
 * Parser.cpp
 *
 *  Created on: 26 апр. 2014
 *      Author: knightl
 */

#include "Parser.h"
#include <cstdlib>

Parser::Parser(const XMLParser& config, xmlNodePtr start):curtime(-1) {
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

