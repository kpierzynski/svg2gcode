#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arc.h"

char * pathx = "m335.8,415.19 l-136.33,61 a6.16,6.16,0,0,0,-3.5,3.57 l-47.48,127.95 a6.13,6.13,0,0,0,7.68,7.95 l283,-104 l16,149.28 l-87.65,102.44 a1.64,1.64,0,0,0,-0.34,1.49 l16.68,62.56 a1.64,1.64,0,0,0,2.7,0.78 l112.32,-103.86 a1.64,1.64,0,0,1,2.24,0 l112.32,103.86 a1.64,1.64,0,0,0,2.7,-0.78 l16.68,-62.56 a1.64,1.64,0,0,0,-0.34,-1.49 L544.6,661 M424.56,374.18 l-9.44,-118.06 a84.77,84.77,0,0,1,169.54,0 l-9.54,119.21 l225.41,100.88 a6.16,6.16,0,0,1,3.5,3.57 l47.48,127.93 a6.13,6.13,0,0,1,-7.68,7.95 L560.56,511.59 L554,572.83";
char * path2 = "m20.0,20.0 m20.0,20.0 l20.0,20.0";
char * path10 = "m33.58,41.519 l-13.633,6.1 a0.616,0.616,0,0,0,-0.35,0.357 l-4.748,12.795 a0.613,0.613,0,0,0,0.768,0.795 l28.3,-10.4 l1.6,14.928 l-8.765,10.244 a0.164,0.164,0,0,0,-0.034,0.149 l1.668,6.256 a0.164,0.164,0,0,0,0.27,0.078 l11.232,-10.386 a0.164,0.164,0,0,1,0.224,0 l11.232,10.386 a0.164,0.164,0,0,0,0.27,-0.078 l1.668,-6.256 a0.164,0.164,0,0,0,-00.34,-0.149 L54.46,66.1 M42.456,37.418 l-0.944,-11.806 a8.477,8.477,0,0,1,16.954,0 l-0.954,11.921 l22.541,10.088 a0.616,0.616,0,0,1,0.35,0.357 l4.748,12.793 a0.613,0.613,0,0,1,-0.768,0.795 L56.056,51.159 L55.4,57.283";

char * path = "M20,20 a10.0,20.0,0,0,0,20.0,0.0";
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
			case 'm':
				Point parsed = parse_point( token + 1 );
				origin.x += parsed.x;
				origin.y += parsed.y;
				last = origin;
				printf("G0 X%f Y%f Z%f F3600 ; cmd: M\r\n", origin.x, origin.y, 0.0);
				break;
			case 'M':
				origin = parse_point( token + 1 );
				last = origin;
				printf("G0 X%f Y%f Z%f F3600 ; cmd: M\r\n", origin.x, origin.y, 0.0);
				break;

			case 'l':
			case 'L':
				Point dest = parse_point( token + 1 );
				if( cmd == 'l') {
					dest.x += last.x;
					dest.y += last.y;
				} 
				last = dest;
				printf("G1 X%f Y%f Z%f E%d ; cmd: L\r\n", dest.x, dest.y, 0.0, e += e_delta);
				break;


			case 'Z':
				printf("G1 X%f Y%f Z%f E%d ; cmd: Z\r\n", origin.x, origin.y, 0.0, e += e_delta);
				break;

			case 'H':
				float _x = parse_coord(token+1);
				printf("G1 X%f Y%f Z%f E%d ; cmd H\r\n", _x, last.y, 0.0, e += e_delta);
				last.x = _x;
				break;

			case 'V':
				float _y = parse_coord(token+1);
				printf("G1 X%f Y%f Z%f E%d ; cmd H\r\n", last.x, _y, 0.0, e += e_delta);
				last.y = _y;
				break;

			case 'C':
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

			case 'a':
				Endpointic arc_data = parse_arc(token+1);
				Centric arc = endpoint2center_parametrization(last, point_add(arc_data.p,last), arc_data.r, arc_data.angle, arc_data.fa, arc_data.fs);

				printf("; %f %f %f %f %f %f (debug: %d)\r\n", arc.r.x, arc.r.y, arc.c.x, arc.c.y, arc.theta, arc.delta, arc_data.fs);

				float t = arc.theta;

				if( arc.theta == arc.delta ) arc_data.fs = 1;

				float x,y; 
				if( arc_data.fs ) {
					while( t < arc.theta+arc.delta ) {
						x = arc.r.x * cos(t) + arc.c.x;
						y = arc.r.y * sin(t) + arc.c.y;
						printf("G1 X%f Y%f Z%f, E%d; cmd: a, fs=1\r\n", x, y, 0.0, e += e_delta);

						t += 0.1;
					}
				} else {
					while( t > arc.theta+arc.delta ) {
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
	}

	return 0;
}
