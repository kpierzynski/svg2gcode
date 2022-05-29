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

char *parse_point(char *s, Point *p);
char *parse_coord(char *s, float * c);

void gcode_move(Point p);
void gcode_draw(Point p);

Point point_add(Point a, Point b);
Point point_subtract(Point a, Point b);
Point point_reflection(Point a, Point origin);

char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);

#endif