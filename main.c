#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arc.h"
#include "bezier.h"
#include "common.h"
#include "loader.h"

char *path = "M20.0,20.0 L40.0,20.0       L40.0,40.0ZM10.0 10.0";
char *path1 = "M20.0,20.0 L40.0,20.0,40.0,40.0 Z";
char *path2 = "M 20.0,20.0 L 40.0,20.0 L 40.0,40.0 Z";
char *path3 = "M 20.0 20.0 L 40.0 20.0 L 40.0 40.0 Z";
char *path4 = "M20.0,20.0L40.0,20.0L40.0,40.0Z";
char *path5 = "M20.0 20.0L40.0 20.0L40.0 40.0Z";

char * parser( char ** s, char * delims, char * delim ) {
	if( **s == NULL ) 
		return NULL;

	int n = strcspn(*s + 1, delims) + 1;

	char * token = (char*)malloc( n );

	memcpy( token, *s, n );
	token[n] = '\0';

	*s += n;
	return token;
}

int main()
{

	char *rest, *oryg;
	oryg = strdup(path);
	rest = trim(oryg);

	Point origin = {0, 0};
	Point last = {0, 0};

	int e = 0;
	int e_delta = 10;

	printf("G90\r\n");

	char *commands = "MZL";

	char * token;
	char delim;

	while( (token = parser(&rest, commands, &delim ) ) ) {
		printf("|%c|:|%s|\r\n", delim, token);
		free(token);
	}

	return 0;
	while (*rest != '\0')
	{
		char cmd;
		char * token;

		int n = strcspn(rest + 1, commands) + 1;

		token = (char *)malloc(n);
		memcpy(token, rest, n);
		token[n] = '\0';

		printf("%c => token: |%s| %d \r\n", *token, trim(token + 1), n);

		cmd = *token;
		token = trim(token);
		//token powinien zaczynac sie za cmd i byc wtedy trimowany!!!

		switch (cmd)
		{
		case 'm':
			Point parsed = parse_point(token + 1);
			origin.x += parsed.x;
			origin.y += parsed.y;
			last = origin;
			printf("G0 X%f Y%f Z%f F3600 ; cmd: M\r\n", origin.x, origin.y, 0.0);
			break;
		case 'M':
			origin = parse_point(token + 1);
			last = origin;
			printf("G0 X%f Y%f Z%f F3600 ; cmd: M\r\n", origin.x, origin.y, 0.0);
			break;

		case 'l':
		case 'L':
			Point dest = parse_point(token + 1);
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
			float _x = parse_coord(token + 1);
			printf("G1 X%f Y%f Z%f E%d ; cmd h\r\n", last.x + _x, last.y, 0.0, e += e_delta);
			last.x += _x;
			break;
		}
		case 'H':
		{
			float _x = parse_coord(token + 1);
			printf("G1 X%f Y%f Z%f E%d ; cmd H\r\n", _x, last.y, 0.0, e += e_delta);
			last.x = _x;
			break;
		}

		case 'v':
		{
			float _y = parse_coord(token + 1);
			printf("G1 X%f Y%f Z%f E%d ; cmd v\r\n", last.x, last.y + _y, 0.0, e += e_delta);
			last.y += _y;
			break;
		}

		case 'V':
			float _y = parse_coord(token + 1);
			printf("G1 X%f Y%f Z%f E%d ; cmd H\r\n", last.x, _y, 0.0, e += e_delta);
			last.y = _y;
			break;

		case 'C':
		{
			Point p1, p2, p3;
			parse_cubic(token + 1, &p1, &p2, &p3);

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
			parse_cubic(token + 1, &p1, &p2, &p3);

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
			Endpointic arc_data = parse_arc(token + 1);
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

		rest += n;
		free(token);
	}

	return 0;
}
