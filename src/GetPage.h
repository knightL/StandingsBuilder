/*
 * GetPage.h
 *
 *  Created on: 25 апр. 2014
 *      Author: knightl
 */

#ifndef GETPAGE_H_
#define GETPAGE_H_

#include <cstdlib>

struct MemoryStruct {
  char *memory;
  size_t size;
};

MemoryStruct DownloadURL(const char* url);

#endif /* GETPAGE_H_ */
