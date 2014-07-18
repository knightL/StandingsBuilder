/*
 * FileReader.h
 *
 *      Author: knightl
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include "config.h"
#include "XMLParser.h"
#include <string>

struct MemoryStruct {
  char *memory;
  size_t size;
};

#ifdef HAVE_LIBCURL
MemoryStruct DownloadURL(const char* url);
#endif

class FileReader
{
	
	std::string path;

	MemoryStruct file;

	void readFile();
#ifdef HAVE_LIBCURL
	void readURL();
#endif
public:
	enum SourceType
	{
		None,
		FileSource
		#ifdef HAVE_LIBCURL
		,URLSource
		#endif
	} sourcetype;
	FileReader(const XMLParser& config, xmlNodePtr start);
	~FileReader();
	const char* read();
	SourceType getType() const;
};

#endif /* FILEREADER_H_ */
