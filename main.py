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



path_strings = ["m16.0332,1.01172 c-2.83359,0,-5.21696,1.98985,-5.83789,4.64063 C9.46415,4.66648,8.30217,4.01563,6.99023,4.01563 c-2.19729,0,-4,1.8027,-4,4 c0,1.18819,0.537243,2.24934,1.36914,2.98438 H2 c-0.552262,0.000055,-0.999945,0.447738,-1,1 v18 c0.0000552,0.552262,0.447738,0.999945,1,1 h21 c0.552262,-0.000055,0.999945,-0.447738,1,-1 v-2.99805 h1.1875 C25.6054,28.1583,26.7077,29,28,29 h2 c0.552262,-0.000055,0.999945,-0.447738,1,-1 V15 c-0.000055,-0.552262,-0.447738,-0.999945,-1,-1 h-2 c-1.29443,0,-2.39816,0.844549,-2.81445,2.00391 H24 V12 c-0.000055,-0.552262,-0.447738,-0.999945,-1,-1 h-2.50195 c0.951374,-1.06117,1.53711,-2.4576,1.53711,-3.98828 c0.000009,-3.30187,-2.70008,-6,-6.00195,-6 Z m0,2 c2.22099,0,4.00196,1.77904,4.00195,4 c0.000006,2.17484,-1.71088,3.9164,-3.86719,3.98828 h-0.269531 c-2.15616,-0.07205,-3.86524,-1.81355,-3.86523,-3.98828 c-0.000006,-2.22096,1.77901,-4,4,-4 Z M6.99023,6.01563 c1.11641,0,2,0.883565,2,2 c4.00000e-7,1.1164,-0.883586,2.00195,-2,2.00195 c-1.11641,0,-2,-0.885556,-2,-2.00195 c0,-1.11643,0.883586,-2,2,-2 Z M10.6445,9.62695 C10.8895,10.1273,11.2,10.5891,11.5684,11 H9.62109 C10.0521,10.6193,10.4079,10.1581,10.6445,9.62695 Z M3,13 H22 V29 H3 Z m25,3 h1 v11 h-1 c-0.565608,0,-0.987669,-0.421627,-0.996094,-0.984375 v-9.03125 C27.0123,16.4216,27.4344,16,28,16 Z m-4,1.99609 h1 v7.00586 h-1 Z"]

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