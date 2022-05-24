#include "arc.h"

float min( float a, float b ) {
    return a > b ? b : a;
}

float max( float a, float b ) {
    return a > b ? a : b;
}

float clamp( float value, float _min, float _max) {
    return max( min(value, _max) , _min);
}

Point point_add(Point a, Point b) {
    return (Point){a.x+b.x, a.y+b.y};
}

Endpointic parse_arc( char * str ) {
    Endpointic arc;

    char * next;
    arc.r.x = strtof( str, &next );
    arc.r.y = strtof( next + 1, &next );

    arc.angle = strtof( next + 1, &next );
    
    arc.fa = strtol( next + 1, &next, 10 );
    arc.fs = strtol( next + 1, &next, 10 );

    arc.p.x = strtof( next + 1, &next );
    arc.p.y = strtof( next + 1, NULL );

    return arc;
}

Centric endpoint2center_parametrization( Point p1, Point p2, Vector2 r, float angle, bool fa, bool fs ) {
    float rx = fabs(r.x);
    float ry = fabs(r.y);

    float dx2 = (p1.x-p2.x)/2;
    float dy2 = (p1.y-p2.y)/2;
    float x1p = cos(angle)*dx2 + sin(angle)*dy2;
    float y1p = -sin(angle)*dx2 + cos(angle)*dy2;

    float rxs = rx*rx;
    float rys = ry*ry;

    float x1ps = x1p*x1p;
    float y1ps = y1p*y1p;

    float cr = x1ps/rxs + y1ps/rys;

    if( cr > 1 ) {
        float s = sqrt(cr);
        rx = s*rx;
        ry = s*ry;
        rxs = rx*rx;
        rys = ry*ry;
    }

    float dq = rxs * y1ps + rys*x1ps;
    float pq = (rxs*rys - dq)/dq;

    float q = sqrt( max(0, pq) );

    if( fa == fs )
        q = -q;

    float cxp = q*rx * y1p/ry;
    float cyp = -q*ry*x1p/rx;

    float cx = cos(angle)*cxp - sin(angle)*cyp + (p1.x+p2.x)/2;
    float cy = sin(angle)*cxp + cos(angle)*cyp + (p1.y+p2.y)/2;

    float theta = angle_function(1, 0, (x1p-cxp)/rx, (y1p-cyp)/ry);
    float delta = angle_function( (x1p-cxp)/rx, (y1p-cyp)/ry, (-x1p-cxp)/rx, (-y1p-cyp)/ry);

    //if( !fs ) 
        //delta -= 2*M_PI;

    Centric cen = { {rx, ry}, {cx, cy}, theta, delta };
    return cen;
}

float angle_function( float ux, float uy, float vx, float vy ) {
    float dot = ux*vx + uy*vy;
    float len = sqrt(ux*ux + uy*uy) * sqrt(vx*vx + vy*vy);
    float ang = acos( clamp(dot/len, -1, 1) );

    if( (ux*vy - uy*vx) < 0 ) {
        ang = -ang;
    }

    return ang;
}
