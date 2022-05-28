#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char * argv[]) {

	char * p;
	float x = strtof(argv[1], &p);
	while( *p == ',' ) p++;
	float y = strtof(p, &p);

	printf("Result: %f %f, str: |%s|\r\n", x, y,p);

	return 0;
}
