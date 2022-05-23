#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * path = "M55.618,52.214 C56.052,52.555 56.128,53.184 55.786,53.618 C55.589,53.869 55.296,54 54.999,54 C54.783,54 54.565,53.931 54.382,53.786 L40.382,42.786 C39.948,42.445 39.872,41.817 40.214,41.382 C40.555,40.949 41.181,40.871 41.618,41.214 L55.618,52.214 Z M19.786,41.382 C20.128,41.817 20.052,42.445 19.618,42.786 L5.618,53.786 C5.434,53.931 5.217,54 5.001,54 C4.704,54 4.411,53.869 4.214,53.618 C3.872,53.184 3.948,52.555 4.382,52.214 L18.382,41.214 C18.818,40.871 19.446,40.949 19.786,41.382 L19.786,41.382 Z M55.606,27.795 L33.648,44.545 C33.485,44.678 31.803,46 30,46 C28.206,46 26.537,44.628 26.352,44.472 L4.395,27.796 C3.955,27.462 3.869,26.835 4.204,26.396 C4.538,25.954 5.165,25.87 5.604,26.204 L27.604,42.913 C27.994,43.238 29.098,44 30,44 C30.929,44 32.057,43.258 32.414,42.972 L54.393,26.205 C54.832,25.867 55.462,25.956 55.795,26.394 C56.13,26.833 56.046,27.46 55.606,27.795 L55.606,27.795 Z M58,55 C58,56.71 56.71,58 55,58 L5,58 C3.317,58 2,56.683 2,55 L2,23 C2,20.683 2.915,18.81 4.555,17.831 L26.561,3.111 C26.579,3.1 28.34,2 30,2 C31.673,2 33.401,3.06 33.402,3.06 L55.476,17.853 C57.562,19.133 58,21.359 58,23 L58,55 Z M56.556,16.169 L34.49,1.381 C34.4,1.325 32.262,-2.13162821e-14 30,-2.13162821e-14 C27.732,-2.13162821e-14 25.572,1.369 25.466,1.439 L3.488,16.141 C1.271,17.46 -6.39488462e-14,19.96 -6.39488462e-14,23 L-6.39488462e-14,55 C-6.39488462e-14,57.804 2.196,60 5,60 L55,60 C57.804,60 60,57.804 60,55 L60,23 C60,19.958 58.765,17.526 56.556,16.169 L56.556,16.169 Z";
char * path2 = "M20,20 L30,30";
char * path1 = "M55.618,52.214 C56.052,52.555 56.128,53.184 55.786,53.618 C55.589,53.869 55.296,54 54.999,54 C54.783,54 54.565,53.931 54.382,53.786 L40.382,42.786 C39.948,42.445 39.872,41.817 40.214,41.382 C40.555,40.949 41.181,40.871 41.618,41.214 L55.618,52.214 Z";

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

void print_point(Point p) {
	printf("; %f %f\r\n", p.x, p.y);
}

Point cubic_bezier(Point p0, Point p1, Point p2, Point p3, float t) {
	float x = (1-t)*(1-t)*(1-t)*p0.x + 3*(1-t)*(1-t)*t*p1.x + 3*(1-t)*t*t*p2.x + t*t*t*p3.x;
	float y = (1-t)*(1-t)*(1-t)*p0.y + 3*(1-t)*(1-t)*t*p1.y + 3*(1-t)*t*t*p2.y + t*t*t*p3.y;

	Point p = {x,y};
	return p;
}

int main(int argc, char *argv[]) {

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

			case 'C':
				Point p1 = parse_point( token + 1 );
				token = strtok_r(rest, " ", &rest);
				Point p2 = parse_point( token );
				token = strtok_r(rest, " ", &rest);
				Point p3 = parse_point( token );

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
