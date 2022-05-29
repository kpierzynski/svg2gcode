#include "common.h"

char *parse_coord(char *s, float *c)
{
	if (*s == '\0')
		return NULL;

	char *next;
	*c = strtof(s, &next);

	return next;
}

char *parse_point(char *s, Point *p)
{
	if (*s == '\0')
		return NULL;

	char *next;
	p->x = strtof(s, &next);

	while (*next == ',')
		next++;
	p->y = strtof(next, &next);
	while (*next == ',')
		next++;

	return next;
}

void gcode_move(Point p) {
	fprintf(stdout, "G0 X%f Y%f Z%f\r\n", p.x, p.y, 0.0f );
}

void gcode_draw(Point p) {
	static int e = 0;
	const int e_delta = 10.0f;

	fprintf(stdout, "G1 X%f Y%f Z%f E%d\r\n", p.x, p.y, 0.0, e += e_delta);
}

Point point_add(Point a, Point b)
{
    return (Point){a.x + b.x, a.y + b.y};
}

Point point_subtract(Point a, Point b)
{
	return (Point){a.x - b.x, a.y - b.y};
}

Point point_reflection(Point a, Point origin) {
	return (Point){ 2*origin.x - a.x, 2*origin.y - a.y };
}

char *ltrim(char *s)
{
	while (isspace(*s))
		s++;
	return s;
}

char *rtrim(char *s)
{
	char *back = s + strlen(s);
	while (isspace(*--back))
		;
	*(back + 1) = '\0';
	return s;
}

char *trim(char *s)
{
	return rtrim(ltrim(s));
}