/*
 * main.cpp
 *
 *      Author: knightl
 */

#include "config.h"
#ifdef HAVE_LIBCURL
#include <curl/curl.h>
#endif
#include "Unroller.h"
#include "GenericParserBuilder.h"
#include "Parsers/EjudgeParser.h"
#include "Parsers/NEERCXmlParser.h"
#include "Parsers/NEERCHtmlParser.h"
#include "Parsers/NYParser.h"
#include "Parsers/Timer.h"

int main(int argc, char** argv)
{
	Unroller unroller;

	// Register all parsers here
	unroller->registerParser<EjudgeParser>();
	unroller->registerParser<NEERCXmlParser>();
	unroller->registerParser<NEERCHtmlParser>();
	unroller->registerParser<NYParser>();
	unroller->registerParser<Timer>();

	unroller->init(argc,argv);

	unroller->run();
}


