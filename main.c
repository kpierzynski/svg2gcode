#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arc.h"
#include "bezier.h"
#include "common.h"

void absolute_gcode()
{
	printf("G90\r\n");
}

void relative_gcode()
{
	printf("G91\r\nM82\r\n");
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

void p(char *path)
{
	printf("PATH: %s\r\n", path);
}

int main()
{

	char *rest, *oryg;
	oryg = strdup("m 50 0 C 55 2.5 55 7.5 50 10");

	rest = trim(oryg);

	Point origin = {0, 0};
	Point last = {0, 0};

	int e = 0;
	int e_delta = 10;

	absolute_gcode();

	char *commands = "mMcCzZlLvVhHa";

	char *token;
	char delim;

	while ((token = parser(&rest, commands, &delim)))
	{
		printf("; (%p) |%c|:|%s|\r\n", token, delim, token + 1);

		char *args = trim(token + 1);
		char *next = args;

		Point p;

		switch (delim)
		{
		case 'm':
			relative_gcode();
		case 'M':
		{
			int cnt = 0;

			while ((next = parse_points(next, &p)))
			{
				printf("; point.x: %f, point.y: %f\r\n", p.x, p.y);
				last = p;
				if (cnt == 0)
				{
					printf("G0 X%f Y%f Z%f F3600 ; cmd: M\r\n", p.x, p.y, 0.0);
					origin = p;
					last = origin;
				}
				else
				{
					printf("G1 X%f Y%f Z%f E%d ; cmd: L\r\n", last.x, last.y, 0.0, e += e_delta);
				}
				cnt++;
			}
			absolute_gcode();
			break;
		}

		case 'l':
			relative_gcode();
		case 'L':
		{
			while ((next = parse_points(next, &p)))
			{
				printf("G1 X%f Y%f Z%f E%d ; cmd: L\r\n", p.x, p.y, 0.0, e += e_delta);
				last = p;
			}
			absolute_gcode();
			break;
		}

		case 'z':
		case 'Z':
		{
			printf("G1 X%f Y%f Z%f E%d ; cmd: Z\r\n", origin.x, origin.y, 0.0, e += e_delta);
			last = origin;
			break;
		}

		case 'h':
			relative_gcode();
		case 'H':
		{
			float c;
			while ((next = parse_coord(next, &c)))
			{
				// printf("G1 X%f Y%f Z%f E%d ; (c=%f, next:%s) cmd h\r\n", c, last.y, 0.0, e += e_delta, c, next);
				printf("G1 X%f Z%f E%d ; (c=%f, next:%s) cmd h\r\n", c, 0.0, e += e_delta, c, next);
				last.x = c;
			}
			absolute_gcode();
			break;
		}

		case 'v':
			relative_gcode();
		case 'V':
		{
			float c;
			while ((next = parse_coord(next, &c)))
			{
				// printf("G1 X%f Y%f Z%f E%d ; (c=%f, next:%s) cmd h\r\n", c, last.y, 0.0, e += e_delta, c, next);
				printf("G1 Y%f Z%f E%d ; (c=%f, next:%s) cmd h\r\n", c, 0.0, e += e_delta, c, next);
				last.y = c;
			}
			absolute_gcode();
			break;
		}

		case 'C':
		{
			Point p1, p2, p3;
			fprintf(stderr, ";last: %f %f\r\n", last.x,last.y);
			while ((next = parse_cub(next, &p1, &p2, &p3)))
			{
				float delta = 0.1;
				printf("; p0.x: %f, p0.y: %f, p1.x: %f, p1.y: %f, p2.x: %f, p2.y: %f, p3.x: %f, p3.y: %f\r\n", last.x, last.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

				for (float t = 0; t <= 1.0f; t += delta)
				{
					Point res = cubic_bezier(last, p1, p2, p3, t);
					printf("G1 X%f Y%f Z%f E%d ; cmd: C\r\n", res.x, res.y, 0.0, e += e_delta);
				}
				last = p3;
				printf("G1 X%f Y%f Z%f E%d ; cmd: C ; return \r\n", last.x, last.y, 0.0, e += e_delta);
			}
			absolute_gcode();
			break;
		}

		case 'c':
		{
			Point p1, p2, p3;
			while ((next = parse_cub(next, &p1, &p2, &p3)))
			{
				float delta = 0.1;
				printf("; p0.x: %f, p0.y: %f, p1.x: %f, p1.y: %f, p2.x: %f, p2.y: %f, p3.x: %f, p3.y: %f\r\n", last.x, last.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

				for (float t = 0; t <= 1.0f; t += delta)
				{
					Point res = cubic_bezier((Point){0,0}, p1, p2, p3, t);
					res = point_add(res, last);
					printf("G1 X%f Y%f Z%f E%d ; cmd: C\r\n", res.x, res.y, 0.0, e += e_delta);
				}
				last = point_add(last,p3);
				printf("G1 X%f Y%f Z%f E%d ; cmd: C ; return \r\n", last.x, last.y, 0.0, e += e_delta);
			}
			absolute_gcode();
			break;
		}

			/*

				case 'a':
					Endpointic arc_data = parse_arc(args);
					Centric arc = endpoint2center_parametrization(last, point_add(arc_data.p, last), arc_data.r, arc_data.angle, arc_data.fa, arc_data.fs);

					printf("; %f %f %f %f %f %f (debug: %d)\r\n", arc.r.x, arc.r.y, arc.c.x, arc.c.y, arc.theta, arc.delta, arc_data.fs);

					float t = arc.theta;

					if (arc.theta == arc.delta)
						arc_data.fs = 1;

					float x, y;
					if (arc_data.fs)
					{
						while (t < arc.theta + arc.delta)
						{
							x = arc.r.x * cos(t) + arc.c.x;
							y = arc.r.y * sin(t) + arc.c.y;
							printf("G1 X%f Y%f Z%f, E%d; cmd: a, fs=1\r\n", x, y, 0.0, e += e_delta);

							t += 0.1;
						}
					}
					else
					{
						while (t > arc.theta + arc.delta)
						{
							x = arc.r.x * cos(t) + arc.c.x;
							y = arc.r.y * sin(t) + arc.c.y;
							printf("G1 X%f Y%f Z%f E%d; cmd: a, fs=0\r\n", x, y, 0.0, e += e_delta);

							t -= 0.1;
						}
					}
					last.x = x;
					last.y = y;
					break;
				*/

		default:
			printf("; UNHANDLED COMMAND: %c\r\n", delim);
			break;
		}

		free(token);
	}

	return 0;
}
