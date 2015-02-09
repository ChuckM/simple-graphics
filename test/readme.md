Functional Tests
----------------

This directory contains some functional tests for the graphics
routines. Since wiring up an actual display can be time consuming
and introduce its own errors, these tests use an "ASCII" display
where "colors" are represented by printable characters. The display
buffer is just a place to store text.

At the end of each test the display buffer is sent to the standard
output and displays on your screen. If you're using an xterm or other
soft terminal screen then set the width to at least 100 characters
so that the terminal application isn't wrapping lines.

**basic.c**

> This is a basic test of the graphic routines. It uses both
> small and large fonts and, as well as font scaling (x2) and
> rotation, it also draws various geometric shapes in both 
> outline form and filled form. Finally it has a triangle test
> where it draws two triangles that share an edge to verify
> that the edge painting of triangles doesn't leave gaps.
>
> It has one option `-m` which will "mirror" the display which
> means to read it you need to see it as a reflection. This is
> useful when making heads up or other projection displays.

**rotation.c**

> This test does screen rotations. The user supplied `draw_pixel`
> function always gets X and Y co-ordinates in display space so
> it doesn't have to change when the screen is rotated. In the
> center of the display is the string 'Up^' which shows you what
> direction the code considers to be "up". 

**large-font.c**

> This test just dumps out the large font so you can see what it
> looks like (and if it has bits wrong :-). Note that it prints
> offset by one (so glyphs 1 - 127, then glyph 0) this is to avoid
> putting a NUL character in a C string causing `gfx_puts` to stop
> printing. 

**small-font.c**

> This test dumps out the small font so that you can see what it
> looks like (and fix it if the bits are in the wrong place). Like
> large-font it puts glyph 0 on the end.
