all:
	gcc -g -Wall -I/usr/include/libxml2 -o main main.c common.c loader.c commands/*.c -lm -lxml2
