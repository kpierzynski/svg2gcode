#ifndef __BEZIER_H_
#define __BEZIER_H_

#include "../common.h"

void svg_cubic(uint8_t is_relative, char *args, Point *initial_point, Point *current_point);
void svg_cubic_s(uint8_t is_relative, char *args, Point *initial_point, Point *current_point);

#endif