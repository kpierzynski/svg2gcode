#include "bezier.h"

Point cubic_bezier(Point p0, Point p1, Point p2, Point p3, float t)
{
    float x = (1 - t) * (1 - t) * (1 - t) * p0.x + 3 * (1 - t) * (1 - t) * t * p1.x + 3 * (1 - t) * t * t * p2.x + t * t * t * p3.x;
    float y = (1 - t) * (1 - t) * (1 - t) * p0.y + 3 * (1 - t) * (1 - t) * t * p1.y + 3 * (1 - t) * t * t * p2.y + t * t * t * p3.y;

    Point p = {x, y};
    return p;
}

char *parse_cub(char *s, Point *p1, Point *p2, Point *p3)
{
    if (*s == '\0')
        return NULL;

    char *next;
    p1->x = strtof(s, &next);

    while (*next == ',')
        next++;

    p1->y = strtof(next, &next);

    while (*next == ',')
        next++;
    p2->x = strtof(next, &next);

    while (*next == ',')
        next++;
    p2->y = strtof(next, &next);

    while (*next == ',')
        next++;
    p3->x = strtof(next, &next);

    while (*next == ',')
        next++;
    p3->y = strtof(next, &next);

    while (*next == ',')
        next++;

    return next;
}

char *parse_ref_cub(char *s, Point *p1, Point *p2)
{
    if (*s == '\0')
        return NULL;

    char *next;
    p1->x = strtof(s, &next);

    while (*next == ',')
        next++;

    p1->y = strtof(next, &next);

    while (*next == ',')
        next++;
    p2->x = strtof(next, &next);

    while (*next == ',')
        next++;
    p2->y = strtof(next, &next);

    while (*next == ',')
        next++;

    return next;
}