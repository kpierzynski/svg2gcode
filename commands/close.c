#include "close.h"

void svg_close(uint8_t is_relative, char *args, Point *initial_point, Point *current_point) {
	gcode_draw(*initial_point);
	*current_point = *initial_point;
}