/*
 * FileReader.cpp
 *
 *      Author: knightl
 */

#include "FileReader.h"

#include <cstdlib>
#include <cstring>

#ifdef HAVE_LIBCURL

#include <curl/curl.h>

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

MemoryStruct DownloadURL(const char* url)
{
	CURL *curl_handle;
	CURLcode res;
	MemoryStruct chunk;
	chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */

	/* init the curl session */
	curl_handle = curl_easy_init();

	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);

	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	/* some servers don't like requests that are made without a user-agent
		 field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	/* get it! */
	res = curl_easy_perform(curl_handle);

	/* check for errors */
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		if(chunk.memory)
			free(chunk.memory);
		chunk.memory=NULL;
	}

	curl_easy_cleanup(curl_handle);

	return chunk;
}
#endif /* HAVE_LIBCURL */

FileReader::FileReader(const XMLParser& config, xmlNodePtr start)
{
	xmlNodePtr cur;
	
	if(config.haveAttribute(start,"Path"))
	{
		path=config.getAttributeContent(start,"Path");
		sourcetype=FileSource;
	}
#ifdef HAVE_LIBCURL
	else if(config.haveAttribute(start,"URL"))
	{
		path=config.getAttributeContent(start,"URL");
		sourcetype=URLSource;
	}
#endif /* HAVE_LIBCURL */
	else
	{
		char* path=(char*)xmlGetNodePath(start);
		printf("%s: missing Path "
#ifdef HAVE_LIBCURL
		"or URL "
#endif
 "attribute\n", path);
		xmlFree((xmlChar*)path);
		sourcetype=None;
	}
	file.memory=NULL;
	file.size=0;
}

FileReader::~FileReader()
{
	if(file.memory)
		free(file.memory);
	file.memory=NULL;
}

const char* FileReader::read()
{
	switch(sourcetype)
	{
	case None:
		break;
	case FileSource:
		readFile();
		break;
#ifdef HAVE_LIBCURL
	case URLSource:
		readURL();
		break;
#endif /* HAVE_LIBCURL */
	}
	return file.memory;
}

void FileReader::readFile()
{
	if(file.memory)
		free(file.memory);
	file.memory=NULL;
	file.size=0;
	FILE *fin=fopen(path.c_str(),"rb");
	if(!fin)
	{
		printf("Failed to open file %s\n",path.c_str());
		return;
	}

	long size;

	// get the file size
	fseek(fin, 0, SEEK_END);
	size = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	char *buf=(char*)calloc(size+1,sizeof(char));
	fread(buf,1,size,fin);
	fclose(fin);

	file.memory=buf;
	file.size=size;
}

#ifdef HAVE_LIBCURL
void FileReader::readURL()
{
	file=DownloadURL(path.c_str());
}
#endif /* HAVE_LIBCURL */

FileReader::SourceType FileReader::getType() const
{
	return sourcetype;
}

