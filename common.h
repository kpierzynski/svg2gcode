#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef uint8_t bool;

typedef struct {
    float x,y;
} Point;

typedef struct {
    float x,y;
} Vector2;

typedef struct {
    Vector2 r;
    Point c;
    float theta;
    float delta;
} Centric;

typedef struct {
    Vector2 r;
    float angle;
    bool fa;
    bool fs;
    Point p;
} Endpointic;

Point parse_point(char * str);
float parse_coord(char * str);

char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);

#endif