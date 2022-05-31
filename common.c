#include "common.h"

float ax = 1, ay = 1;
float bx, by;

void replace_comma(char *s)
{
	do
	{
		if (*s == ',')
			*s = ' ';
	} while (*s++);
}

char * parse_floats(char *s, float *f, int n)
{
	if (*s == '\0')
		return NULL;

	char *k = s;
	for (int i = 0; i < n; i++)
	{
		*f++ = strtof(k, &k);
	}

	return k;
}

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

void set_function_x(float _a, float _b)
{
	ax = _a;
	bx = _b;
}

void set_function_y(float _a, float _b)
{
	ay = _a;
	by = _b;
}

void apply_function(Point *p)
{
	p->x = ax * p->x + bx;
	p->y = ay * p->y + by;
};

void gcode_move(Point p)
{
	apply_function(&p);
	fprintf(stdout, "G0 X%f Y%f Z%f\r\n", p.x, p.y, 0.0f);
}

void gcode_draw(Point p)
{
	static int e = 0;
	const int e_delta = 10.0f;

	apply_function(&p);
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

Point point_reflection(Point a, Point origin)
{
	return (Point){2 * origin.x - a.x, 2 * origin.y - a.y};
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