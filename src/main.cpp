/*
 * main.cpp
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#include <curl/curl.h>
#include "Unroller.h"
#include "GenericParserBuilder.h"
#include "Parsers/EjudgeLinkParser.h"
#include "Parsers/EjudgeFileParser.h"
#include "Parsers/NEERCXmlParser.h"
#include "Parsers/NEERCHtmlParser.h"

int main(int argc, char** argv)
{
	curl_global_init(CURL_GLOBAL_ALL);
	Unroller unroller;
	unroller->registerParser<EjudgeLinkParser>();
	unroller->registerParser<EjudgeFileParser>();
	unroller->registerParser<NEERCXmlParser>();
	unroller->registerParser<NEERCHtmlParser>();
	unroller->init(argc,argv);
	unroller->run();
}


