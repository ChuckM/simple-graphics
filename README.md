Simple Graphics Library
-----------------------

There is a bounty of cheap color LCD displays these days. Driving them
requires some direct bit twiddling to do basic things like draw lines
or circles or characters on them. The simple graphics library has a
collection of these functions which are generally useful and not too
heavyweight.

This library is used in my projects primarily on ARM Cortex-M class
machines. These are 32 bit machines with generally > 64K of flash and
a reasonable amount of RAM. Some have multi-megabytes of RAM! This is
a short way of saying that this doesn't fit well into 8-bit
microcontrollers and I am ok with that, there are other libraries that
do.

As an LCD driver where you can create a non-complex UI the following
capabilities are provided:

 * Write text to the screen in a large or small font, scale the
   font as desired but increasing the size of pixels.
 * Text rotation of an arbitrary number of degrees is supported.
 * Draw filled or open triangles, with colinear edges that correctly
   rasterize. So if you're building a pattern out of triangles it will
   paint correctly.
 * Draw filled or open circles by specifying center and radius.
 * Draw rectangles with square or rounded corners, either filled or
   open.
 * Apply a rotation before drawing, this allows you to either write
   "sideways" on an display or create a 45 degree display if you want.
 * Mirror the display so if your graphics are viewed through a mirror
   as they are on a "heads up" type display they appear correct to
   the viewer.
 * Minimal porting, the code is written to the c99 spec and should be
   portable so you just supply your own
   `draw_pixel(void *, int, int, uint16_t)` function. See more below.
 * A set of basic tests that use ASCII as their color pallete so you
   can experiment on a machine without display hardware.

## Architecture, Porting, and Compatibility

The library tries to be self contained without depending on external
definitions. The exception is that it uses the C 'standard int' types.
It also uses bit fields and so the 'raw' versions of things will depend
on endianness. Not a lot of testing has been done on non-ARM machines.

While many LCDs have limited color palettes, this library uses a
ARGB8888 (Alpha, Red, Blue, and Green components, 8 bits each) color
type which it defines as a `GFX_COLOR`. It is up to the pixel storing
function that you supply to convert it to something your display device
understands.

When you initialize the library, you get to pass in a private pointer
which will be passed along to the pixel writing function when it is
called. That can point to what ever you want and it allows multiple
instances of the library to use the same pixel drawing function.

When the user supplied pixel drawing function is called, it has been
transformed and clipped into 'display space' so you can count on its
X and Y co-ordinates to be within the range of 0 to width-1 and 0 to
height-1. Even if the display is rotated the X and Y values your function
gets are based on the width and height you passed in during initialization.

## One Page API Reference

All functions take a graphics context (`GFX_CTX`) when you call them.
This has information telling the function things like transforms, colors,
and where the current 'cursor' is. There are two cursors maintained, one
is the draw cursor and one is the text cursor. You can set either cursor
to an absolute co-ordinate using `gfx_move_to` (draw cursor) or 
`gfx_set_text_cursor` (text cursor).

### Utility
  * `GFX_CTX *gfx_init(draw_pixel, width, height, font_spec, private_pointer)` -- This must be
	called first and sets up the width and height of the display and
	sets the font. Current font choices are `GFX_FONT_TINY` (5x7),
	`GFX_FONT_SMALL` (6x8), and `GFX_FONT_LARGE` (8x12).
  * `gfx_fill_screen(ctx, color)` -- Color is a `GFX_COLOR` and is passed
	unmolested to the `draw_pixel` function. See the test code where
	I use it to pass ASCII character values as "colors" in my
	test code.
  * `gfx_rotate(ctx, angle)` -- Rotate the display by `angle` degrees.
    The center of this rotation is the center of the screen by default
	but you can change that with the next function.
  * `gfx_set_rotation_origin(ctx, x, y)` -- Set the origin (about which
	rotations occur) to co-ordinates `[x,y]` in display space.
  * `gfx_set_mirrored(ctx, flag)` -- If `flag` is non-zero it will set
	the display to mirrored mode (x axis is reversed). If `flag` is 0
	it will reset mirrored mode. This is useful when your
	display is being mirrored first as in a heads up display.
  * `gfx_move(GFX_CTX, int x, int y)` -- Move the drawing cursor without
    drawing from where it was, to where it was +x, +y.
  * `gfx_move_to(GFX_CTX, int x, int y)` -- Move the drawing cursor to
     x, y.

### Drawing
  * `gfx_draw(GFX_CTX *g, x1, y1, GFX_COLOR)` -- Draw a line from
	the current drawing cursor location to the point that is +x1, +y1
	pixels from the current location.
  * `gfx_draw_to(GFX_CTX *g, x2, y2, GFX_COLOR)` -- Draw a line from
	the current draw cursor co-ordinates to the point [x2,y2] on the
	screen. Note that drawing _includes_ the end point. So the pixel
	at [x2, y2] will be painted with the color specified.
  * `gfx_draw_point(GFX_CTX *, color)` -- Draw a point (pixel) in color 
	`color` at the current draw cursor's location.
  * `gfx_draw_point_at(GFX_CTX *, x, y, color)` -- Draw a point 
	 (pixel) in color `color` at the location [x, y].
  * `gfx_draw_rectangle(GFX_CTX *g, width, height, color)` -- Draws a
	rectangle that is `width` pixels by `height` pixels at the current
	drawing cursor's co-ordinates. 
  * `gfx_draw_rectangle_at(GFX_CTX *g, x, y,width, height, color)` -- Draws a
	rectangle that is `width` pixels by `height` pixels at [x, y] on the
	screen.
  * `gfx_draw_filled_rectangle(GFX_CTX *g, width, height, color)` -- Draws
	a filled rectangle that is `width` pixels by `height` pixels at the 
	current drawing cursor's co-ordinates.
  * `gfx_draw_filled_rectangle_at(GFX_CTX *g, x, y, width, height, color)` -- Draws
	a filled rectangle that is `width` pixels by `height` pixels at the 
	location [x, y] on the screen.
  * `gfx_draw_rounded_rectangle(GFX_CTX *g, width, height, radius, color)` --
	Draw a rectangle with rounded corners (corner radius `radius`) in color
	`color` at the current drawing cursor's co-ordinates.
  * `gfx_draw_rounded_rectangle_at(GFX_CTX *g, x, y, width, height, radius, color)` --
	Draw a rectangle with rounded corners (corner radius `radius`) in color
	`color` at location [x, y] on the screen.
  * `gfx_fill_rounded_rectangle(GFX_CTX *g, width, height, radius, color)` --
	Draw a filled rounded corner rectangle.
  * `gfx_fill_rounded_rectangle_at(GFX_CTX *g, x, y, width, height, radius, color)` --
	Draw a filled rounded corner rectangle at location [x, y] on the screen.
  * `gfx_draw_circle(GFX_CTX *g, radius, color)` -- Draw a circle of radius
	`radius` centered at the current co-ordinates.
  * `gfx_draw_circle_at(GFX_CTX *g, x, y, radius, color)` -- Draw a circle 
	of radius `radius` centered at co-ordinates x, y.
  * `gfx_fill_circle(GFX_CTX *g, radius, color)` -- Draw a filled circle
	of radius `radius` centered at current drawing cursor.
  * `gfx_fill_circle_at(GFX_CTX *g, x, y, radius, color)` -- Draw a filled 
	circle of radius `radius` centered at location [x, y] on the screen.
  * `gfx_draw_triangle(GFX_CTX *g, x1, y1, x2, y2, color)` -- Draws a 
	triangle between (cursor) -> (x1, y1) -> (x2, y2). Cursor is unchanged.
  * `gfx_draw_triangle_at(GFX_CTX *g, x, y, x1, y1, x2, y2, color)` -- Draws
	a triangle between points (x,y) -> (x1, y1) -> (x2, y2), cursor becomes
	[x, y].
  * `gfx_fill_triangle(GFX_CTX *ctx, x1, y1, x2, y2, color)` -- Draws a 
	filled triangle between points (cursor) -> (x1, y1) -> (x2, y2). Cursor
	is unchanged.
  * `gfx_fill_triangle_at(GFX_CTX *ctx, x, y, x1, y1, x2, y2, color)` -- 
	Draws a filled triangle between points (x, y) -> (x1, y1) -> (x2, y2).
	The drawing cursor is updated to (x, y).

### Text
  * `gfx_setTextColor(FGColor, BGColor)` -- Set the foreground and
	background colors for text. If `BGColor` is zero then the 
    background is unchanged. Use a background color of 0x1 to write
    black as the background color.
  * `gfx_setFont(FontSpec)` -- You can switch between the large and
    the small font.
  * `gfx_puts(message)` -- Writes the message to the screen at the
	current cursor location, moving the cursor right by one text
	box for each character. Will drop down one line and return to
	the left most co-ordinate from `gfx_setCursor` if wrapping is
	enabled.
  * `gfx_putc(c)` -- Writes a character on the screen and advances
	the co-ordinates in the text cursor by one text box position.
  * `gfx_textSize(size)` -- Applies an integer scaling value to all
    future text. No anti-aliasing is done, just big blocky letters.
  * `gfx_textWrap(on/off)` -- Sets text wrapping on or off, when
    it is off text off screen is simply clipped and discarded.
  * `gfx_getTextHeight()` -- Returns the height of text so you can
    move to the next line down by adding this value to your Y
	co-ordinate.
  * `gfx_getTextWidth()` -- Returns the size of a text box which
	lets you compute how wide a string will be when printed by
	multiplying this value by its length.
  * `gfx_setCursor(x0, y0)` -- Sets the text cursor to the indicated
	co-ordinates. This should be the bottom left corner of the text
	box.
  * `gfx_textRotate(rotation)` -- Allows text to be rotated to one
	of four directions, 0 (normal), 90 straight down with the top
	of the text to the right, 180 upside down and right to left,
	and 270 which is straight up with the top of the text pointed
	left.



### Color Defines

The following color defines are provided in `gfx.h`, they are based
on an RGB565 Color pallete.

```
#define GFX_COLOR_WHITE          0xFFFF
#define GFX_COLOR_BLACK          0x0000
#define GFX_COLOR_GREY           0xF7DE
#define GFX_COLOR_BLUE           0x001F
#define GFX_COLOR_BLUE2          0x051F
#define GFX_COLOR_RED            0xF800
#define GFX_COLOR_MAGENTA        0xF81F
#define GFX_COLOR_GREEN          0x07E0
#define GFX_COLOR_CYAN           0x7FFF
#define GFX_COLOR_YELLOW         0xFFE0
```
