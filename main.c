#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//char * path = "M55.618,52.214 C56.052,52.555 56.128,53.184 55.786,53.618 C55.589,53.869 55.296,54 54.999,54 C54.783,54 54.565,53.931 54.382,53.786 L40.382,42.786 C39.948,42.445 39.872,41.817 40.214,41.382 C40.555,40.949 41.181,40.871 41.618,41.214 L55.618,52.214 Z M19.786,41.382 C20.128,41.817 20.052,42.445 19.618,42.786 L5.618,53.786 C5.434,53.931 5.217,54 5.001,54 C4.704,54 4.411,53.869 4.214,53.618 C3.872,53.184 3.948,52.555 4.382,52.214 L18.382,41.214 C18.818,40.871 19.446,40.949 19.786,41.382 L19.786,41.382 Z M55.606,27.795 L33.648,44.545 C33.485,44.678 31.803,46 30,46 C28.206,46 26.537,44.628 26.352,44.472 L4.395,27.796 C3.955,27.462 3.869,26.835 4.204,26.396 C4.538,25.954 5.165,25.87 5.604,26.204 L27.604,42.913 C27.994,43.238 29.098,44 30,44 C30.929,44 32.057,43.258 32.414,42.972 L54.393,26.205 C54.832,25.867 55.462,25.956 55.795,26.394 C56.13,26.833 56.046,27.46 55.606,27.795 L55.606,27.795 Z M58,55 C58,56.71 56.71,58 55,58 L5,58 C3.317,58 2,56.683 2,55 L2,23 C2,20.683 2.915,18.81 4.555,17.831 L26.5
char * path = "m335.8 415.19-136.33 61a6.16 6.16 0 0 0-3.5 3.57l-47.48 127.95a6.13 6.13 0 0 0 7.68 7.95l283-104 16 149.28-87.65 102.44a1.64 1.64 0 0 0-.34 1.49l16.68 62.56a1.64 1.64 0 0 0 2.7.78l112.32-103.86a1.64 1.64 0 0 1 2.24 0l112.32 103.86a1.64 1.64 0 0 0 2.7-.78l16.68-62.56a1.64 1.64 0 0 0-.34-1.49L544.6 661M424.56 374.18l-9.44-118.06a84.77 84.77 0 0 1 169.54 0l-9.54 119.21 225.41 100.88a6.16 6.16 0 0 1 3.5 3.57l47.48 127.93a6.13 6.13 0 0 1-7.68 7.95L560.56 511.59 554 572.83";

typedef struct {
	float x;
	float y;
} Point;

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

void print_point(Point p) {
	printf("; %f %f\r\n", p.x, p.y);
}

Point cubic_bezier(Point p0, Point p1, Point p2, Point p3, float t) {
	float x = (1-t)*(1-t)*(1-t)*p0.x + 3*(1-t)*(1-t)*t*p1.x + 3*(1-t)*t*t*p2.x + t*t*t*p3.x;
	float y = (1-t)*(1-t)*(1-t)*p0.y + 3*(1-t)*(1-t)*t*p1.y + 3*(1-t)*t*t*p2.y + t*t*t*p3.y;

	Point p = {x,y};
	return p;
}

int main() {

	char * token, *rest, *oryg;
	oryg = strdup(path);
	rest = oryg;

	Point origin = {0,0};
	Point last = {0,0};

	int e = 0;
	int e_delta = 10;

	printf("G90\r\n");

	while( (token = strtok_r(rest, " ", &rest) ) ) {
		char cmd = *token;

		printf("; token: %s, cmd: %c\r\n", token, cmd);
		fflush(stdout);
		switch(cmd) {
			case 'M':
				origin = parse_point( token + 1 );
				last = origin;
				printf("G0 X%f Y%f Z%f F3600 ; cmd: M\r\n", origin.x, origin.y, 0.0);
				break;

			case 'L':
				Point dest = parse_point( token + 1 );
				last = dest;
				printf("G1 X%f Y%f Z%f E%d ; cmd: L\r\n", dest.x, dest.y, 0.0, e += e_delta);
				break;

			case 'Z':
				printf("G1 X%f Y%f Z%f E%d ; cmd: Z\r\n", origin.x, origin.y, 0.0, e += e_delta);
				break;

			case 'H':
				float x = parse_coord(token+1);
				printf("G1 X%f Y%f Z%f E%d ; cmd H\r\n", x, last.y, 0.0, e += e_delta);
				last.x = x;
				break;

			case 'V':
				float y = parse_coord(token+1);
				printf("G1 X%f Y%f Z%f E%d ; cmd H\r\n", last.x, y, 0.0, e += e_delta);
				last.y = y;
				break;

			case 'c':
				printf("G91\r\n");
			case 'C':
				if( cmd == 'C' ) printf("G90\r\n");
				Point p1 = parse_point( token + 1 );
				token = strtok_r(rest, " ", &rest);
				Point p2 = parse_point( token );
				token = strtok_r(rest, " ", &rest);
				Point p3 = parse_point( token );

				print_point(p1);
				print_point(p2);
				print_point(p3);

				float delta = 0.1f;
				for( float t = 0; t <= 1.0f; t += delta ) {
					Point res = cubic_bezier(last, p1, p2, p3, t);
					printf("G1 X%f Y%f Z%f E%d ; cmd: C\r\n", res.x, res.y, 0.0, e += e_delta);
				}

				last = p3;
				printf("G0 X%f Y%f Z%f F3600 ; cmd: C ; return \r\n", p3.x, p3.y, 0.0);
				break;
		}
	}

	return 0;
}
