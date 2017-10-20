bin_PROGRAMS = standingsbuilder
standingsbuilder_SOURCES =  src/Contest.cpp \
                            src/Contest.h \
			    src/FileReader.cpp \
                            src/FileReader.h \
                            src/GenericParserBuilder.cpp \
                            src/GenericParserBuilder.h \
                            src/luacontestlib.cpp \
                            src/luacontestlib.h \
                            src/main.cpp \
                            src/Parser.cpp \
                            src/Parser.h \
                            src/Result.cpp \
                            src/Result.h \
                            src/singletone.h \
                            src/Team.cpp \
                            src/Team.h \
                            src/Unroller.cpp \
                            src/Unroller.h \
                            src/XMLParser.cpp \
                            src/XMLParser.h \
                            src/Parsers/EventBasedParser.cpp\
                            src/Parsers/EventBasedParser.h\
                            src/Parsers/EjudgeParser.cpp\
                            src/Parsers/EjudgeParser.h\
                            src/Parsers/NEERCXmlParser.cpp\
                            src/Parsers/NEERCXmlParser.h\
                            src/Parsers/NEERCHtmlParser.cpp\
                            src/Parsers/NEERCHtmlParser.h\
                            src/Parsers/NYParser.h\
                            src/Parsers/NYParser.cpp\
                            src/Parsers/Timer.cpp\
                            src/Parsers/Timer.h
AM_CPPFLAGS = @libcurl_CFLAGS@ @libxml_CFLAGS@ @lua5_2_CFLAGS@
standingsbuilder_LDADD = @libcurl_LIBS@ @libxml_LIBS@ @lua5_2_LIBS@
EXTRA_DIST = outputSimpleStyle.lua outputEjudgeStyle.lua
