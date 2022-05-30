#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loader.h"
#include "common.h"

#include "commands/move.h"
#include "commands/line.h"
#include "commands/close.h"
#include "commands/arc.h"
#include "commands/bezier.h"

void absolute_gcode()
{
	printf("G90 ; absolute\r\n");
}

void relative_gcode()
{
	printf("G91 ; relative\r\nM82\r\n");
}

char *parser(char **s, char *delims, char *delim)
{
	if (**s == '\0')
		return NULL;

	int n = strcspn(*s + 1, delims) + 1;
	char *token = (char *)malloc(n + 1);

	memcpy(token, *s, n);
	token[n] = '\0';

	*delim = *token;

	*s += n;
	return token;
}

void parse(char *path)
{
	char *rest, *oryg;
	oryg = strdup(path);
	rest = trim(oryg);

	Point origin = {0, 0};
	Point last = {0, 0};

	Point second_control = {0, 0};

	absolute_gcode();

	char *commands = "mMcCzZlLvVhHaAsSqQtT";

	char *token;
	char delim;

	while ((token = parser(&rest, commands, &delim)))
	{
		printf("; (%p) |%c|:|%s|\r\n", token, delim, token + 1);

		char *args = trim(token + 1);

		uint8_t relative = islower(delim);

		switch (delim)
		{
		case 'm':
			svg_move(1, args, &origin, &last);
			break;
		case 'M':
			svg_move(0, args, &origin, &last);
			break;

		case 'l':
			svg_line(1, args, &origin, &last);
			break;
		case 'L':
			svg_line(0, args, &origin, &last);
			break;

		case 'h':
			svg_lineH(1, args, &origin, &last);
			break;
		case 'H':
			svg_lineH(0, args, &origin, &last);
			break;

		case 'v':
			svg_lineV(1, args, &origin, &last);
			break;
		case 'V':
			svg_lineV(0, args, &origin, &last);
			break;

		case 'z':
			svg_close(1, args, &origin, &last);
			break;
		case 'Z':
			svg_close(0, NULL, &origin, &last);
			break;

		case 'a':
			svg_arc(1, args, &origin, &last);
			break;
		case 'A':
			svg_arc(0, args, &origin, &last);
			break;

		case 'c':
			svg_cubic(1, args, &origin, &last);
			break;
		case 'C':
			svg_cubic(0, args, &origin, &last);
			break;
		}

		free(token);
	}
}

int main(int argc, char *argv[])
{
	// char *path = "M 10.0 10.0 20.0 10.0 20.0 20.0 m -10 0 0 10 M 30.0 10.0 40.0 10.0 "; //Mm TEST
	// char * path = "m 10 10 L 20 10 20 20 10 30 l -10 0 m 30 0 h -10 -10 v 10 10 M 0 0 H 20 30 "; //LlhHvV test
	// char * path = "M 10 10 h 10 v 10 z"; //zZ test
	//char * path = "m 0 7 a 10 5 0 0 0 8 -7"; //a test
	// char * path = "M 10 10 A 10 5 0 0 1 30 -10 H 40"; //A test
	//char *path = "M 10 10 C 18 14 10 6 18 10"; // cC test
	//char * path = "m 10 10 c 8 4 12 -6 8 0"; //cC test2

	char * plane = "m 335.8 415.19 l -136.33 61 a 6.16 6.16 0 0 0 -3.5 3.57 l -47.48 127.95 a 6.13 6.13 0 0 0 7.68 7.95 l 283 -104 l 16 149.28 l -87.65 102.44 a 1.64 1.64 0 0 0 -0.34 1.49 l 16.68 62.56 a 1.64 1.64 0 0 0 2.7 0.78 l 112.32 -103.86 a 1.64 1.64 0 0 1 2.24 0 l 112.32 103.86 a 1.64 1.64 0 0 0 2.7 -0.78 l 16.68 -62.56 a 1.64 1.64 0 0 0 -0.34 -1.49 L 544.6 661 M 424.56 374.18 l -9.44 -118.06 a 84.77 84.77 0 0 1 169.54 0 l -9.54 119.21 l 225.41 100.88 a 6.16 6.16 0 0 1 3.5 3.57 l 47.48 127.93 a 6.13 6.13 0 0 1 -7.68 7.95 L 560.56 511.59 L 554 572.83";

	parse(plane);

	return 0;
}
