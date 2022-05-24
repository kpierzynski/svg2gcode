#ifndef __ARC_H_
#define __ARC_H_

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

Point point_add(Point a, Point b);
float angle_function( float ux, float uy, float vx, float vy );
Centric endpoint2center_parametrization( Point p1, Point p2, Vector2 r, float angle, bool fa, bool fs );

Endpointic parse_arc( char * str );

#endif