Simple Graphics Library
-----------------------

There is a bounty of cheap color LCD displays these days. Driving them
requires some direct bit twiddling to do basic things like draw lines
or circles or characters on them. The simple graphics library has a
collection of these functions which are generally useful and not too
heavyweight.

As an LCD driver where you can create a non-complex UI the following
capabilities are provided:

 * Write text to the screen in a large or small font, scale the
   font as desired but increasing the size of pixels.
 * Text rotation of 0, 90, 180, and 270 degrees supported.
 * Draw filled or open triangles, with colinear edges that correctly
   rasterize. So if you're building a pattern out of triangles it will
   look correct.
 * Draw filled or open circles by specifying center and radius.
 * Draw rectangles with square or rounded corners, either filled or
   open.
 * Rotate the display so that the code can treat it as if it were
   correctly oriented, even if the LCD was wired up in an
   inconvenient orientation.
 * Mirror the display so if your graphics are viewed through a mirror
   as they are on a "heads up" type display they appear correct to
   the viewer.
 * Minimal porting, the code is portable so you just supply your own
   `draw_pixel(int, int, uint16_t)` function which works in LCD display 
   co-ordinates, pre-mapping based on orientation for you, and
   pre-clipped so the pixel writer doesn't get passed invalid X or Y
   values.
 * A set of basic tests that use ASCII as their color pallete so you
   can experiment on a machine without display hardware.
