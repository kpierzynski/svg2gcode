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
