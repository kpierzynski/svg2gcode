all:
	gcc -g -Wall -I/usr/include/libxml2 -o main main.c arc.c bezier.c common.c loader.c -lm -lxml2
