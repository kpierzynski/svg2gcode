#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loader.h"
#include "common.h"

#include "commands/move.h"
#include "commands/line.h"
#include "commands/close.h"
#include "commands/arc.h"

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
			case 'Z':
				svg_close(0, NULL, &origin, &last);
				break;

			case 'a':
				svg_arc(1, args, &origin, &last);
				break;
			case 'A':
				svg_arc(0, args, &origin, &last);
				break;
		}

		free(token);
	}
}

int main(int argc, char *argv[])
{
	//char *path = "M 10.0 10.0 20.0 10.0 20.0 20.0 m -10 0 0 10 M 30.0 10.0 40.0 10.0 "; //Mm TEST
	//char * path = "m 10 10 L 20 10 20 20 10 30 l -10 0 m 30 0 h -10 -10 v 10 10 M 0 0 H 20 30 "; //LlhHvV test
	//char * path = "M 10 10 h 10 v 10 z"; //zZ test
	//char * path = "m 10 10 a 10 5 0 0 1 20 -20 h 20"; //a test
	//char * path = "M 10 10 A 10 5 0 0 1 30 -10 H 40"; //A test
	parse(path);

	return 0;
}
