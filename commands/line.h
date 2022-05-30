#ifndef __LINE_H_
#define __LINE_H_

#include "../common.h"

void svg_line(uint8_t is_relative, char *args, Point *initial_point, Point *current_point);
void svg_lineH(uint8_t is_relative, char *args, Point *initial_point, Point *current_point);
void svg_lineV(uint8_t is_relative, char *args, Point *initial_point, Point *current_point);

#endif