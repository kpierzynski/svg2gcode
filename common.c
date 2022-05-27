#include "common.h"

char *parse_coord(char *s, float * c)
{
	if (*s == '\0')
		return NULL;

	char *next;
	*c = strtof(s, &next);

	return next;
}

char *parse_points(char *s, Point *p)
{
	if (*s == '\0')
		return NULL;

	char *next;
	float x = strtof(s, &next);
	float y = strtof(next + 1, &next);
	p->x = x;
	p->y = y;

	return next;
}

Point point_subtract(Point a, Point b) {
	return (Point){ a.x-b.x, a.y-b.y };
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