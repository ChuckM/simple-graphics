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

## One Page API Reference

The most useful API calls in a single, easy to use reference.

### Utility
  * `gfx_init(draw_pixel, width, height, font_spec)` -- This must be
	called first and sets up the width and height of the display and
	chooses either `GFX_FONT_LARGE` which is a 7x9 font in an 8 x 12
	character "box", or `GFX_FONT_SMALL` which is a 5x6 font in a 6
	x 8 character box.
  * `gfx_fillScreen(color)` -- Color is a `uint16_t` and is passed
	unmolested to the `draw_pixel` function. This is pretty flexible
	and I use it to pass ASCII character values as "colors" in my
	test code.
  * `gfx_setRotation(rotation)` -- Rotate the co-ordinate space of
    the display. This applies one of four rotations to the output
    so (0, 90, 180, and 270) degrees. Each has their own define 
    of the form `GFX_ROTATE_<val>`. Non-90 degree rotations are not
    supported.
  * `gfx_setMirrored(on/off)` -- This will mirror the output (reverse
	it left to right but leave up/down unchanged.) useful when your
	display is being mirrored first as in a heads up display.

### Drawing
  * `gfx_drawLine(x0, y0, x1, y1, color)` -- Draw a line from (x0,y0)
	to (x1, y1) in color `color`.
  * `gfx_drawPixel(x0, y0, color)` -- Draw a point (pixel) in color 
	`color` at co-ordinates x0, y0.
  * `gfx_drawRect(x0, y0, width, height, color)` -- Draw a rectangle
	that is `width` pixels by `height` pixels at co-ordinates (x0, y0).
  * `gfx_fillRect(x0, y0, width, height, color)` -- Draw a filled
	rectangle that is `width` pixels by `height` pixels at co-ordinates
	(x0, y0).
  * `gfx_drawRoundRect(x0, y0, width, height, radius, color)` -- Draw
	a rectangle with rounded corners (corner radius `radius`).
  * `gfx_fillRoundRect(x0, y0, width, height, radius, color)` -- Draw
	a filled rounded corner rectangle.
  * `gfx_drawCircle(x0, y0, radius, color)` -- Draw a circle of radius
	`radius` centered at co-ordinates x0, y0.
  * `gfx_fillCircle(x0, y0, radius, color)` -- Draw a filled circle
	of radius `radius` centered at co-ordinates x0, y0.
  * `gfx_drawTriangle(x0, y0, x1, y1, x2, y2, color)` -- Draw a triangle
	between points (x0,y0), (x1, y1), and (x2, y2).
  * `gfx_fillTriangle(x0, y0, x1, y1, x2, y2, color)` -- Draw a 
	filled triangle between points (x0,y0), (x1, y1), and (x2, y2).
	Note that the points should be in "clockwise" order for the fill
	algorithm to work.
  * `gfx_drawBitmap(x0, y0, bitmap, w, h, color)` -- Draws a "bitmap"
	which is a `uint8_t` array that is `width` * `height` bytes in size.
	each byte is looked at and the library writes `color` if that byte
	is non-zero. 

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
