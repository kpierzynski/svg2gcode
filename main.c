#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arc.h"
#include "bezier.h"
#include "common.h"
#include "loader.h"

char *path = "m 16.0332 1.01172 c -2.83359 0 -5.21696 1.98985 -5.83789 4.64063 C 9.46415 4.66648 8.30217 4.01563 6.99023 4.01563 c -2.19729 0 -4 1.8027 -4 4 c 0 1.18819 0.537243 2.24934 1.36914 2.98438 H 2 c -0.552262 0.000055 -0.999945 0.447738 -1 1 v 18 c 0.0000552 0.552262 0.447738 0.999945 1 1 h 21 c 0.552262 -0.000055 0.999945 -0.447738 1 -1 v -2.99805 h 1.1875 C 25.6054 28.1583 26.7077 29 28 29 h 2 c 0.552262 -0.000055 0.999945 -0.447738 1 -1 V 15 c -0.000055 -0.552262 -0.447738 -0.999945 -1 -1 h -2 c -1.29443 0 -2.39816 0.844549 -2.81445 2.00391 H 24 V 12 c -0.000055 -0.552262 -0.447738 -0.999945 -1 -1 h -2.50195 c 0.951374 -1.06117 1.53711 -2.4576 1.53711 -3.98828 c 0.000009 -3.30187 -2.70008 -6 -6.00195 -6 Z m 0 2 c 2.22099 0 4.00196 1.77904 4.00195 4 c 0.000006 2.17484 -1.71088 3.9164 -3.86719 3.98828 h -0.269531 c -2.15616 -0.07205 -3.86524 -1.81355 -3.86523 -3.98828 c -0.000006 -2.22096 1.77901 -4 4 -4 Z M 6.99023 6.01563 c 1.11641 0 2 0.883565 2 2 c 4.00000e-7 1.1164 -0.883586 2.00195 -2 2.00195 c -1.11641 0 -2 -0.885556 -2 -2.00195 c 0 -1.11643 0.883586 -2 2 -2 Z M 10.6445 9.62695 C 10.8895 10.1273 11.2 10.5891 11.5684 11 H 9.62109 C 10.0521 10.6193 10.4079 10.1581 10.6445 9.62695 Z M 3 13 H 22 V 29 H 3 Z m 25 3 h 1 v 11 h -1 c -0.565608 0 -0.987669 -0.421627 -0.996094 -0.984375 v -9.03125 C 27.0123 16.4216 27.4344 16 28 16 Z m -4 1.99609 h 1 v 7.00586 h -1 Z";
char *path00 = "M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0M20.0,20.0 L 40.0,20.0       L40.0,40.0ZM10.0 10.0";
char *path1 = "M20.0,20.0 L40.0,20.0,40.0,40.0 Z";
char *path2 = "M 20.0,20.0 L 40.0,20.0 L 40.0,40.0 Z";
char *path3 = "M 20.0 20.0 L 40.0 20.0 L 40.0 40.0 Z";
char *path4 = "M20.0,20.0L40.0,20.0L40.0,40.0Z";
char *path5 = "M20.0 20.0L40.0 20.0L40.0 40.0Z";

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
	printf("PATH: %s\r\n",path);
}

int main()
{

	load_paths("bell.svg", p);
	return 0;

	char *rest, *oryg;
	oryg = strdup(path);
	rest = trim(oryg);

	Point origin = {0, 0};
	Point last = {0, 0};

	int e = 0;
	int e_delta = 10;

	printf("G90\r\n");

	char *commands = "mMcCzZlLvVhHa";

	char *token;
	char delim;

	while ((token = parser(&rest, commands, &delim)))
	{
		printf("; (%p) |%c|:|%s|\r\n", token, delim, token + 1);

		char *args = trim(token + 1);

		char cmd = delim;
		switch (cmd)
		{
		case 'm':
			Point parsed = parse_point(args);
			origin.x += parsed.x;
			origin.y += parsed.y;
			last = origin;
			printf("G0 X%f Y%f Z%f F3600 ; cmd: M\r\n", origin.x, origin.y, 0.0);
			break;
		case 'M':
			origin = parse_point(args);
			last = origin;
			printf("G0 X%f Y%f Z%f F3600 ; cmd: M\r\n", origin.x, origin.y, 0.0);
			break;

		case 'l':
		case 'L':
			Point dest = parse_point(args);
			if (cmd == 'l')
			{
				dest.x += last.x;
				dest.y += last.y;
			}
			last = dest;
			printf("G1 X%f Y%f Z%f E%d ; cmd: L\r\n", dest.x, dest.y, 0.0, e += e_delta);
			break;

		case 'z':
		case 'Z':
			printf("G1 X%f Y%f Z%f E%d ; cmd: Z\r\n", origin.x, origin.y, 0.0, e += e_delta);
			last = origin;
			break;

		case 'h':
		{
			float _x = parse_coord(args);
			printf("G1 X%f Y%f Z%f E%d ; cmd h\r\n", last.x + _x, last.y, 0.0, e += e_delta);
			last.x += _x;
			break;
		}
		case 'H':
		{
			float _x = parse_coord(args);
			printf("G1 X%f Y%f Z%f E%d ; cmd H\r\n", _x, last.y, 0.0, e += e_delta);
			last.x = _x;
			break;
		}

		case 'v':
		{
			float _y = parse_coord(args);
			printf("G1 X%f Y%f Z%f E%d ; cmd v\r\n", last.x, last.y + _y, 0.0, e += e_delta);
			last.y += _y;
			break;
		}

		case 'V':
			float _y = parse_coord(args);
			printf("G1 X%f Y%f Z%f E%d ; cmd H\r\n", last.x, _y, 0.0, e += e_delta);
			last.y = _y;
			break;

		case 'C':
		{
			Point p1, p2, p3;
			parse_cubic(args, &p1, &p2, &p3);

			float delta = 0.1f;
			for (float t = 0; t <= 1.0f; t += delta)
			{
				Point res = cubic_bezier(last, p1, p2, p3, t);
				printf("G1 X%f Y%f Z%f E%d ; cmd: C\r\n", res.x, res.y, 0.0, e += e_delta);
			}

			last = p3;
			printf("G1 X%f Y%f Z%f E%d ; cmd: C ; return \r\n", p3.x, p3.y, 0.0, e += e_delta);
			break;
		}

		case 'c':
		{
			Point p1, p2, p3;
			parse_cubic(args, &p1, &p2, &p3);

			float delta = 0.1f;
			for (float t = 0; t <= 1.0f; t += delta)
			{
				Point res = cubic_bezier(last, point_add(last, p1), point_add(last, p2), point_add(last, p3), t);
				printf("G1 X%f Y%f Z%f E%d ; cmd: c\r\n", res.x, res.y, 0.0, e += e_delta);
			}

			last = point_add(last, p3);
			printf("G1 X%f Y%f Z%f E%d ; cmd: c ; return \r\n", last.x, last.y, 0.0, e += e_delta);
			break;
		}

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

		default:
			printf("; UNHANDLED COMMAND: %c\r\n", cmd);
			break;
		}

		free(token);
	}

	return 0;
}
