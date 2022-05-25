#ifndef __LOADER_H_
#define __LOADER_H_

#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>

void load_svg(const char * path);

#endif