# svg2gcode
Converter from SVG PATHS to GCODE.
This program was created for converting svg pcbs from easyeda to gcode using one command instead of tedious inkscape.

![example_gcode](./images/camera_gcode.png)
![example_svg](./images/camera_svg.png)

---
Usage: `./svg2gcode [filepath2svg]`

---
Path commands:
- &#9745; mM
- &#9745; lL
- &#9745; vV
- &#9745; hH 
- &#9745; zZ
- &#9745; aA
- &#9745; cC
- &#9745; sS
- &#9745; tT
- &#9745; qQ

---
TODO:
* ~~handle rest path commands~~
* reduce code in functions that draw bezier
* reduce code in parser function
* handle ~~polyline~~, ~~polygon~~, circle and ellipse tags
* ~~write proper makefile~~
