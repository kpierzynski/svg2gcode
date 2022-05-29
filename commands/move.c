#include "move.h"

void svg_move(uint8_t is_relative, char *args, Point *initial_point, Point *current_point)
{

	Point parsed;
	char *next = args;

	uint8_t is_line_mode = 0;

	while ((next = parse_point(next, &parsed)))
	{
		if (is_relative)
			parsed = point_add(parsed, *current_point);

		if (is_line_mode)
		{
			gcode_draw(parsed);
			*current_point = parsed;
		}
		else
		{
			gcode_move(parsed);
			*initial_point = parsed;
			*current_point = *initial_point;
		}
		is_line_mode = 1;
	}
}