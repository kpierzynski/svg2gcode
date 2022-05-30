#include "arc.h"

static float min(float a, float b)
{
	return a > b ? b : a;
}

static float max(float a, float b)
{
	return a > b ? a : b;
}

static float clamp(float value, float _min, float _max)
{
	return max(min(value, _max), _min);
}

static char *parse_arc(char *s, Endpointic *arc)
{
	if (*s == '\0')
		return NULL;

	char *next;
	arc->r.x = strtof(s, &next);
	while (*next == ',')
		next++;
	arc->r.y = strtof(next, &next);
	while (*next == ',')
		next++;
	arc->angle = strtof(next, &next);
	while (*next == ',')
		next++;
	arc->fa = strtol(next, &next, 10);
	while (*next == ',')
		next++;
	arc->fs = strtol(next, &next, 10);
	while (*next == ',')
		next++;
	arc->p.x = strtof(next, &next);
	while (*next == ',')
		next++;
	arc->p.y = strtof(next, &next);
	while (*next == ',')
		next++;
	return next;
}

static float angle_function(float ux, float uy, float vx, float vy)
{
	float dot = ux * vx + uy * vy;
	float len = sqrt(ux * ux + uy * uy) * sqrt(vx * vx + vy * vy);
	float ang = acos(clamp(dot / len, -1, 1));

	if ((ux * vy - uy * vx) < 0)
	{
		ang = -ang;
	}

	return ang;
}

static Centric endpoint2center_parametrization(Point p1, Point p2, Vector2 r, float angle, uint8_t fa, uint8_t fs)
{
	float rx = fabs(r.x);
	float ry = fabs(r.y);

	float dx2 = (p1.x - p2.x) / 2;
	float dy2 = (p1.y - p2.y) / 2;
	float x1p = cos(angle) * dx2 + sin(angle) * dy2;
	float y1p = -sin(angle) * dx2 + cos(angle) * dy2;

	float rxs = rx * rx;
	float rys = ry * ry;

	float x1ps = x1p * x1p;
	float y1ps = y1p * y1p;

	float cr = x1ps / rxs + y1ps / rys;

	if (cr > 1)
	{
		float s = sqrt(cr);
		rx = s * rx;
		ry = s * ry;
		rxs = rx * rx;
		rys = ry * ry;
	}

	float dq = rxs * y1ps + rys * x1ps;
	float pq = (rxs * rys - dq) / dq;

	float q = sqrt(max(0, pq));

	if (fa == fs)
		q = -q;

	float cxp = q * rx * y1p / ry;
	float cyp = -q * ry * x1p / rx;

	float cx = cos(angle) * cxp - sin(angle) * cyp + (p1.x + p2.x) / 2;
	float cy = sin(angle) * cxp + cos(angle) * cyp + (p1.y + p2.y) / 2;

	float theta = angle_function(1, 0, (x1p - cxp) / rx, (y1p - cyp) / ry);
	float delta = angle_function((x1p - cxp) / rx, (y1p - cyp) / ry, (-x1p - cxp) / rx, (-y1p - cyp) / ry);

	if (!fs)
		delta -= 2 * M_PI;

	delta = fmod(delta, 2*M_PI);

	Centric cen = {{rx, ry}, {cx, cy}, theta, delta};
	return cen;
}

void svg_arc(uint8_t is_relative, char *args, Point *initial_point, Point *current_point)
{
	Endpointic arc_data;

	char *next = args;

	while ((next = parse_arc(next, &arc_data)))
	{
		Point end = (is_relative) ? point_add(arc_data.p, *current_point) : arc_data.p;
		Centric arc = endpoint2center_parametrization(*current_point, end, arc_data.r, arc_data.angle, arc_data.fa, arc_data.fs);

		printf(";arc_data (endpoint): px: %f py: %f rx: %f ry: %f angle: %f fa: %d fs: %d \r\n\r\n", arc_data.p.x, arc_data.p.y, arc_data.r.x, arc_data.r.y, arc_data.angle, arc_data.fa, arc_data.fs);
		printf(";arc (centric):  rx: %f ry: %f cx: %f cy: %f delta: %f theta: %f\r\n\r\n", arc.r.x, arc.r.y, arc.c.x, arc.c.y, arc.delta, arc.theta);

		Point p;

		for (uint8_t i = 0; i < 100; i++)
		{
			float angle = arc.theta + arc.delta * i / 100;
			float cosr = cos(arc_data.angle);
			float sinr = sin(arc_data.angle);

			p.x = cosr * cos(angle) * arc.r.x - sinr * sin(angle) * arc.r.y + arc.c.x;
			p.y = sinr * cos(angle) * arc.r.x + cosr * sin(angle) * arc.r.y + arc.c.y;

			gcode_draw(p);
		}

		*current_point = p;
	}
}
