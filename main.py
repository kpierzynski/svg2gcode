from svg.path import parse_path
from svg.path.path import Line, CubicBezier, Arc
from xml.dom import minidom

import matplotlib.pyplot as plt
from math import sin,cos, fmod, acos, pi
# read the SVG file
doc = minidom.parse('bell.svg')
path_strings = [path.getAttribute('d') for path
                in doc.getElementsByTagName('path')]
doc.unlink()

def clamp(my_value,min_value,max_value):
	return max(min(my_value, max_value), min_value)

def svgAngle( ux, uy, vx, vy ):
	dot = ux*vx + uy*vy
	len = ((ux*ux + uy*uy)**(1/2))*((vx*vx + vy*vy)**(1/2))
	ang = acos( clamp(dot/len, -1, 1) )

	if (ux*vy-uy*vx) < 0:
		ang = -ang
	return ang	

def end2cen(p1,p2,r,ang,fa,fs):
	p1x, p1y = p1
	p2x, p2y = p2
	rx, ry = r

	rx = abs(rx)
	ry = abs(ry)

	dx2 = (p1x-p2x)/2
	dy2 = (p1y-p2y)/2
	x1p = cos(ang)*dx2 + sin(ang)*dy2
	y1p = -sin(ang)*dx2 + cos(ang)*dy2

	rxs = rx*rx
	rys = ry*ry

	x1ps = x1p*x1p
	y1ps = y1p*y1p

	cr = x1ps/rxs + y1ps/rys

	if cr > 1:
		s = cr**(1/2)
		rx = s*rx
		ry = s*ry
		rxs = rx*rx
		rys = ry*ry

	dq = (rxs * y1ps + rys * x1ps)
	pq = (rxs*rys - dq)/dq

	q = (max(0,pq))**(1/2)

	if fa == fs:
		q = -q

	cxp = q * rx * y1p/ry
	cyp = -q*ry*x1p/rx

	cx = cos(ang)*cxp - sin(ang)*cyp + (p1x + p2x)/2
	cy = sin(ang)*cxp + cos(ang)*cyp + (p1y + p2y)/2

	theta = svgAngle( 1, 0, (x1p-cxp)/rx, (y1p-cyp)/ry)
	delta = svgAngle( (x1p-cxp)/rx, (y1p-cyp)/ry, (-x1p-cxp)/rx, (-y1p-cyp)/ry)
	print("DELTA: " + str(delta))

	delta = fmod(delta, 2*pi)

	#if not fs:
		#delta -= 2*pi

	return rx,ry,cx,cy,theta,delta



path_strings = ["M10.0 10.0 l40.0 0.0 0.0 10.0 l-30.0 0.0 L 10.0 30.0 "]

for path_string in path_strings:
    path = parse_path(path_string)
    for e in path:
        if isinstance(e, CubicBezier):
            for x in [0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9]:
                p1 = e.point( (x+0j) )
                p2 = e.point( ((x+0.1)+0j) )
                x0 = p1.real
                y0 = p1.imag
                x1 = p2.real
                y1 = p2.imag
                plt.plot([x0,x1],[y0,y1])
        elif isinstance(e, Line):
            x0 = e.start.real
            y0 = e.start.imag
            x1 = e.end.real
            y1 = e.end.imag
            plt.plot([x0,x1],[y0,y1])
        elif isinstance(e, Arc):
            s = e.start.real
            q = e.start.imag

            r = e.end.real
            p = e.end.imag

            a = e.radius.real
            b = e.radius.imag

            print(e)

            rx,ry,cx,cy,theta,delta = end2cen( (e.start.real,e.start.imag), (e.end.real,e.end.imag), (a,b), e.rotation, e.arc, e.sweep)

            print(rx,ry,cx,cy,theta,delta,e.sweep)

            step = 0.1
            e.sweep = True

            if not e.sweep:
                t = theta
                while t > theta+delta-step:
                    x1 = rx*cos(t) + cx
                    y1 = ry*sin(t) + cy

                    x2 = rx*cos((t+step)) + cx
                    y2 = ry*sin((t+step)) + cy

                    plt.plot([x1,x2],[y1,y2])
                    t -= step
                continue

            t = theta
            while t < theta+delta-step:
                x1 = rx*cos(t) + cx
                y1 = ry*sin(t) + cy

                x2 = rx*cos((t+step)) + cx
                y2 = ry*sin((t+step)) + cy

                plt.plot([x1,x2],[y1,y2])
                t += step
        else:
            pass
		
plt.show()