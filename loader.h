#ifndef __LOADER_H_
#define __LOADER_H_

#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>

#include "common.h"

void load_paths(const char *path, void(callback)(char *s));

#endif