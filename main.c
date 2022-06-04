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

	absolute_gcode();

	char *commands = "mMcCzZlLvVhHaAsSqQtT";

	char *token;
	char delim;

	while ((token = parser(&rest, commands, &delim)))
	{
		printf("; |%c|:|%s|\r\n", delim, token + 1);

		char *args = trim(token + 1);

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
		
		case 's':
			svg_cubic_s(1, args, &origin, &last);
			break;
		case 'S':
			svg_cubic_s(0, args, &origin, &last);
			break;

		case 'q':
			svg_quadratic(1, args, &origin, &last);
			break;
		case 'Q':
			svg_quadratic(0, args, &origin, &last);
			break;

		case 't':
			svg_quadratic_s(1, args, &origin, &last);
			break;
		case 'T':
			svg_quadratic_s(0, args, &origin, &last);
			break;
		}
		free(token);
	}
	reset_cubic();
}

int main(int argc, char *argv[])
{
	if( argc != 2 ) {
		fprintf(stderr, "Invalid arguments. Usage: %s [filepath]", argv[0] );
		return 1;
	}

	load_paths(argv[1], parse);

	// char *path = "M 10.0 10.0 20.0 10.0 20.0 20.0 m -10 0 0 10 M 30.0 10.0 40.0 10.0 "; //Mm TEST
	//char * path = "m 10 10 L 20 10 20 20 10 30 l -10 0 m 30 0 h -10 -10 v 10 10 M 0 0 H 20 30 "; //LlhHvV test
	// char * path = "M 10 10 h 10 v 10 z"; //zZ test
	//char * path = "m 0 7 a 10 5 0 0 0 8 -7"; //a test
	// char * path = "M 10 10 A 10 5 0 0 1 30 -10 H 40"; //A test
	//char *path = "M 10 10 C 18 14 10 6 18 10"; // cC test
	//char * path = "m 10 10 c 8 4 12 -6 8 0"; //cC test2

	//char * path = "m 1 4 c 0 5 6 3 10 0 s -10 -8 -10 -3";

	//char * plane = "m 335.8 415.19 l -136.33 61 a 6.16 6.16 0 0 0 -3.5 3.57 l -47.48 127.95 a 6.13 6.13 0 0 0 7.68 7.95 l 283 -104 l 16 149.28 l -87.65 102.44 a 1.64 1.64 0 0 0 -0.34 1.49 l 16.68 62.56 a 1.64 1.64 0 0 0 2.7 0.78 l 112.32 -103.86 a 1.64 1.64 0 0 1 2.24 0 l 112.32 103.86 a 1.64 1.64 0 0 0 2.7 -0.78 l 16.68 -62.56 a 1.64 1.64 0 0 0 -0.34 -1.49 L 544.6 661 M 424.56 374.18 l -9.44 -118.06 a 84.77 84.77 0 0 1 169.54 0 l -9.54 119.21 l 225.41 100.88 a 6.16 6.16 0 0 1 3.5 3.57 l 47.48 127.93 a 6.13 6.13 0 0 1 -7.68 7.95 L 560.56 511.59 L 554 572.83";

	//char * path = "M362.54,328.74c-0.01-0.01-0.01-0.01-0.02-0.02l-100.71-87.67c-2.63-2.29-6.56-2.29-9.19,0l-98.48,85.73    l-2.23,1.94c-0.01,0.01-0.01,0.01-0.02,0.02l-17.22,14.99l-20.53-23.58l62.23-54.18c2.92-2.54,3.22-6.96,0.68-9.88    c-2.54-2.92-6.96-3.22-9.88-0.68l-67.51,58.78c-2.92,2.54-3.22,6.96-0.68,9.88l29.72,34.14c1.22,1.4,2.94,2.26,4.8,2.39    c1.87,0.13,3.68-0.48,5.08-1.7l118.63-103.27l89.69,78.08l1.8,1.57l2.22,1.93v120.19h-93.62v-91.85c0-3.87-3.13-7-7-7h-50.15    c-3.87,0-7,3.13-7,7v91.85H163.5v-87.74c0-3.87-3.13-7-7-7s-7,3.13-7,7v94.74c0,3.87,3.13,7,7,7h43.66h50.15h107.62    c3.87,0,7-3.13,7-7v-115l10.92,9.5c1.28,1.11,2.91,1.72,4.6,1.72c0.16,0,0.32-0.01,0.48-0.02c1.85-0.13,3.58-0.99,4.8-2.39    l29.72-34.14c2.54-2.92,2.23-7.34-0.68-9.88l-56.25-48.98v-71.83c0-3.87-3.13-7-7-7h-39.83c-3.87,0-7,3.13-7,7v24.96l-42.87-37.33    c-2.63-2.29-6.56-2.29-9.19,0L203,224.22c-2.92,2.54-3.22,6.96-0.68,9.88c2.54,2.92,6.96,3.22,9.88,0.68l45.02-39.2L307.08,239    c2.07,1.8,5,2.23,7.5,1.09c2.5-1.14,4.1-3.63,4.1-6.37v-33.34h25.83v68.01c0,2.02,0.88,3.95,2.4,5.28l53.38,46.47l-20.53,23.58    L362.54,328.74z M207.16,457.38v-84.85h36.15v84.85H207.16z";

	//char * path = "m 10 10 q 5 -10 10 0"; //qQ test
	//char * path = "m 10 10 q 5 -10 10 0 t 3 0"; //tT test
	//parse(path);

	return 0;
}
