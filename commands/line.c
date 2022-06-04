#include "line.h"

void svg_line(uint8_t is_relative, char *args, Point *initial_point, Point *current_point)
{
	Point parsed;

	char *next = args;

	while ((next = parse_point(next, &parsed)))
	{
		if (is_relative)
			parsed = point_add(parsed, *current_point);

		gcode_draw(parsed);

		*current_point = parsed;
	}
}

void svg_lineH(uint8_t is_relative, char *args, Point *initial_point, Point *current_point)
{
	float c;
	Point p = *current_point;

	char *next = args;

	while ((next = parse_floats(next, &c, 1)))
	{
		p.x = (is_relative) ? p.x + c : c;
		gcode_draw(p);

		*current_point = p;
	}
}

void svg_lineV(uint8_t is_relative, char *args, Point *initial_point, Point *current_point)
{
	float c;
	Point p = *current_point;

	char *next = args;

	while ((next = parse_floats(next, &c, 1)))
	{
		p.y = (is_relative) ? p.y + c : c;
		gcode_draw(p);

		*current_point = p;
	}
}