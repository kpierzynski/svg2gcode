#include "common.h"

Point parse_point(char * str) {
	Point p;
	char * next;
	p.x = strtof( str, &next );
	p.y = strtof( next + 1, NULL );
	return p;
}

float parse_coord(char * str) {
	float c = strtof(str, NULL);
	return c;
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