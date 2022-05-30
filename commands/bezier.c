#include "bezier.h"

static Point second_control;

static Point cubic_curve(Point p0, Point p1, Point p2, Point p3, float t)
{
	Point p;
	p.x = (1 - t) * (1 - t) * (1 - t) * p0.x + 3 * (1 - t) * (1 - t) * t * p1.x + 3 * (1 - t) * t * t * p2.x + t * t * t * p3.x;
	p.y = (1 - t) * (1 - t) * (1 - t) * p0.y + 3 * (1 - t) * (1 - t) * t * p1.y + 3 * (1 - t) * t * t * p2.y + t * t * t * p3.y;

	return p;
}

static char *parse_cub(char *s, Point *p1, Point *p2, Point *p3)
{
	if (*s == '\0')
		return NULL;

	char *next;
	p1->x = strtof(s, &next);

	while (*next == ',')
		next++;

	p1->y = strtof(next, &next);

	while (*next == ',')
		next++;
	p2->x = strtof(next, &next);

	while (*next == ',')
		next++;
	p2->y = strtof(next, &next);

	while (*next == ',')
		next++;
	p3->x = strtof(next, &next);

	while (*next == ',')
		next++;
	p3->y = strtof(next, &next);

	while (*next == ',')
		next++;

	return next;
}

static char *parse_ref_cub(char *s, Point *p1, Point *p2)
{
	if (*s == '\0')
		return NULL;

	char *next;
	p1->x = strtof(s, &next);

	while (*next == ',')
		next++;

	p1->y = strtof(next, &next);

	while (*next == ',')
		next++;
	p2->x = strtof(next, &next);

	while (*next == ',')
		next++;
	p2->y = strtof(next, &next);

	while (*next == ',')
		next++;

	return next;
}

void svg_cubic(uint8_t is_relative, char *args, Point *initial_point, Point *current_point)
{
	char *next = args;
	Point p0, p1, p2, p3;

	float delta = 0.1;
	while ((next = parse_cub(next, &p1, &p2, &p3)))
	{
		p0 = (is_relative) ? (Point){0, 0} : *current_point;

		for (float t = 0; t <= 1.0f; t += delta)
		{
			Point result = cubic_curve(p0, p1, p2, p3, t);
			if (is_relative)
				result = point_add(result, *current_point);
			gcode_draw(result);
		}

		if (is_relative)
		{
			second_control = point_add(*current_point, p2);
			*current_point = point_add(*current_point, p3);
		}
		else
		{
			second_control = p2;
			*current_point = p3;
		}
		gcode_draw(*current_point);
	}
}