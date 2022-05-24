#ifndef __COMMON_H_
#define __COMMON_H_

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

#endif