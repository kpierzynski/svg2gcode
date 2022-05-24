#ifndef __CUBIC_H_
#define __CUBIC_H_

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

Point cubic_bezier(Point p0, Point p1, Point p2, Point p3, float t);
void parse_cubic( char * str, Point *p1, Point *p2, Point *p3 );

#endif