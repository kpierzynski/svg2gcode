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
	oryg = strdup("M 15.9824 0.988281 C 14.3374 0.988281 12.9844 2.34133 12.9844 3.98633 C 12.9844 4.8777 13.3902 5.6757 14.0156 6.22852 C 10.0026 7.13403 7 10.7143 7 15 C 7 17.8774 6.96911 21.483 5.30859 25.0918 C 3.99336 25.4088 2.99609 26.5945 2.99609 28 C 2.99609 29.6447 4.35725 31.0039 6.00195 31.0039 L 25.998 31.0039 C 27.6427 31.0039 29.0039 29.6447 29.0039 28 C 29.0039 26.5945 28.0066 25.4088 26.6914 25.0918 C 25.0309 21.483 25 17.8774 25 15 C 25 10.7044 21.984 7.11588 17.957 6.2207 C 18.5775 5.6681 18.9785 4.8736 18.9785 3.98633 C 18.9785 2.34133 17.6274 0.988281 15.9824 0.988281 Z M 15.9824 2.98828 C 16.5465 2.98828 16.9863 3.4222 16.9863 3.98633 C 16.9863 4.55046 16.5466 4.99023 15.9824 4.99023 C 15.4183 4.99023 14.9844 4.55046 14.9844 3.98633 C 14.9844 3.4222 15.4183 2.98828 15.9824 2.98828 Z M 16 8 C 19.8953 8 23 11.1047 23 15 C 23 17.6559 23.0769 21.2674 24.5449 25 L 7.45508 25 C 8.92311 21.2674 9 17.6559 9 15 C 9 11.1047 12.1047 8 16 8 Z M 6.00195 27.002 L 25.998 27.002 C 26.5693 27.002 27.0039 27.4287 27.0039 28 C 27.0039 28.5713 26.5693 29.0039 25.998 29.0039 L 6.00195 29.0039 C 5.43066 29.0039 4.99805 28.5713 4.99805 28 C 4.99805 27.4287 5.43066 27.002 6.00195 27.002 Z");

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
			fprintf(stderr, ";last: %f %f\r\n", last.x, last.y);
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
					Point res = cubic_bezier((Point){0, 0}, p1, p2, p3, t);
					res = point_add(res, last);
					printf("G1 X%f Y%f Z%f E%d ; cmd: C\r\n", res.x, res.y, 0.0, e += e_delta);
				}
				last = point_add(last, p3);
				printf("G1 X%f Y%f Z%f E%d ; cmd: C ; return \r\n", last.x, last.y, 0.0, e += e_delta);
			}
			absolute_gcode();
			break;
		}

		case 'a':
		{
			Endpointic arc_data;
			while ((next = parse_arc(next, &arc_data)))
			{
				Centric arc = endpoint2center_parametrization(last, point_add(arc_data.p, last), arc_data.r, arc_data.angle, arc_data.fa, arc_data.fs);

				float t = arc.theta;

				if (arc.theta == arc.delta)
					arc_data.fs = 1;

				float x, y;

				if (!arc_data.fs)
				{
					t = -t;
					arc.theta = -arc.theta;
					arc.delta = -arc.delta;
				}

				while (t < arc.theta + arc.delta)
				{
					x = arc.r.x * cos(t) + arc.c.x;
					y = arc.r.y * sin(t) + arc.c.y;
					printf("G1 X%f Y%f Z%f, E%d; cmd: a, fs=1\r\n", x, y, 0.0, e += e_delta);

					t += 0.1;
				}

				last = (Point){x, y};
			}

			absolute_gcode();
			break;
		}

				default:
			printf("; UNHANDLED COMMAND: %c\r\n", delim);
			break;
		}

		free(token);
	}

	return 0;
}
