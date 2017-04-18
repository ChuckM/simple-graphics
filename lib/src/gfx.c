/*
 * Simple Graphics
 *
 * Copyright (c) 2014-2015 Charles McManis, all rights reserved.
 *
 * V2
 *
 * This code is reasonably portable, does assume 32 bit integers and
 * includes both a 7x9 as well as a 5x7 font so uses more "ROM" or flash
 * space. 
 * --Chuck McManis
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gfx.h"

#define DEBUG
extern GFX_FONT_GLYPHS small_font;
extern GFX_FONT_GLYPHS large_font;
extern GFX_FONT_GLYPHS tiny_font;


/*
 * Must be called first. Sets up the graphics context and
 * sets the display size.
 */
GFX_CTX *
gfx_init(void (*pixel_func)(void *, int, int, GFX_COLOR), int width, int height, GFX_FONT font, void *fb)
{
	GFX_CTX *res = malloc(sizeof(GFX_CTX)); /* allocate a context */
	memset((uint8_t *)res, 0, sizeof(GFX_CTX));
	res->w			= width;
	res->h			= height;
	res->cr.x	= width / 2;
	res->cr.y	= height / 2; 
	gfx_rotate(res, 0);
	res->cx = res->cy = 0;
	gfx_set_font(res, font);
	res->text.font  = font;
	res->text.magnify  = 1;
	res->text.cx = 0;
	res->text.cy = res->glyphs->height;
	/* black letters on white background */
	res->text.fg = COLOR(0, 0, 0);
	res->text.bg = COLOR(0xff, 0xff, 0xff);
	gfx_set_text_rotation(res, 0);
	res->fb = fb;
	res->drawpixel = pixel_func;
	return res;
}

/*
 * Define some helper routines, the first
 * swaps two ints using the XOR function.
 */
#define swap(a, b) { a ^= b; b ^= a; a ^= b; }

/* return the lowest of three numbers */
#define min(x, y, z) ((((x) < (y)) && ((x) < (z))) ? (x) :\
			(((y) < (z)) ? (y) : (z)))

/* return the largest of three numbers */
#define max(x, y, z) ((((x) > (y)) && ((x) > (z))) ? (x) :\
			(((y) > (z)) ? (y) : (z)))

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))


/*
 * Architectural note: the __paint_xxx functions
 * are specified in absolute co-ordinates and
 * don't depend on the graphic context's [cx, cy]
 * location. Pixels are in 'user space' and the
 * __paint_pixel() function translates them into
 * display space prior to sending them to the
 * pixel drawing callback function. Currently the
 * only transform supported between 'user' space
 * and 'display' space, is rotation consisting of
 *
 * both an axis of rotation (by default the center
 * of the display) and the degree of rotation.
 *
 * In V2, rotations are an arbitrary number of degrees,
 * not just the 'easy' ones of 0, 90, 180, and 270. This
 * is easy because there is FP hardware on the M4F series.
 * but it could be abstracted out perhaps.
 */
static void
__paint_pixel(GFX_CTX *gfx, int x, int y, GFX_COLOR color)
{
	int xt, yt; 	/* transformed versions */
	/*
	 * Transform the co-ordinates from user space
	 * into display space.
	 */
	/* translate to the rotation center */
	x -= gfx->cr.x;
	y -= gfx->cr.y;
	/* rotate */
	xt = x * gfx->cr.xfrm[0][0] + y * gfx->cr.xfrm[1][0];
	yt = x * gfx->cr.xfrm[0][1] + y * gfx->cr.xfrm[1][1];
	/* translate back in the real world */
	x = xt + gfx->cr.x;
	y = yt + gfx->cr.y;

	/* now clip to the screen's actual size */
	if ((x < 0) || (x >= gfx->w) ||
	    (y < 0) || (y >= gfx->h)) {
		return; // off screen so don't draw it
	}

	/*
	 * mirror the screen if requested
 	 * X only? Do we need Y here too?
	 */
	if ((gfx->flags & GFX_DISPLAY_INVERT) != 0) {
		x = (gfx->w - 1) - x;
	}

	/* invoke user's callback in display co-ordinates */
	(gfx->drawpixel)(gfx->fb, x, y, color);
}

/*
 * Internal function to paint the line from [x0,y0]
 * to [x1, y1]. Uses Bresenham's algorithm and detects
 * horizontal and vertical lines. Updating [cx, cy] is
 * the responsibility of the calling function.
 *
 * Important note: Paint line is *inclusive* so if you
 * paint from 0 to 9 you get 10 pixels, off by one errors
 * abound here so be careful.
 */
static void
__paint_line(GFX_CTX *g, int x0, int y0, int x1, int y1, GFX_COLOR color) {
	int mx, my, e;
	int da = abs(x1 - x0) > abs(y1 - y0);

	/* One pixel line if the destination is the same */
	if ((x0 == x1) && (y0 == y1)) {
		__paint_pixel(g, x0, y0, color);
		return;
	}

	/* 
	 * Check for and deal with horizontal and vertical lines
	 * (slope is 0 or infinity). NB these are horizontal or
	 * vertical in user space only, they may be diagonal on
 	 * the display if a rotation has been applied.
	 */
	if (x0 == x1) {
		if (y0 > y1) {
			e = y0; y0 = y1; y1 = e;
		}
		while (y0 <= y1) {
			__paint_pixel(g, x0, y0, color);
			y0++;
		}
		return;
	}

	if (y0 == y1) {
		if (x0 > x1) {
			e = x0; x0 = x1; x1 = e;
		}
		while (x0 <= x1) {
			__paint_pixel(g, x0, y0, color);
			x0 ++;
		}
		return;
	}

	/*
	 * Now the interesting ones, DA is the "driving axis"
	 *
	 * DA == TRUE (x is the driving axis)
	 *       FALSE (y is the driving axis)
 	 * TODO Could collapse the drawing part and just swap
	 *		x,y in the __paint_pixel call based on DA if
	 * 		I wanted.
	 */
	if (da) {
		/* if X1 is left of X0, swap co-ords */
		if (x1 < x0) {
			e = x0;	/* borrow it as a temp */
			x0 = x1;
			x1 = e;
			e = y0;
			y0 = y1;
			y1 = e;
		}
		mx = x1 - x0;
		my = abs(y1 - y0);
		e = my - mx;
		while (x0 <= x1) {
			__paint_pixel(g, x0, y0, color);
			x0++;
			e += my; 
			if (e > 0) {
				y0 += (y0 > y1) ? -1 : 1;
				e -= mx;
			}
		}
	} else {
		/* swap coords if Y1 is less than Y0 */
		if (y1 < y0) {
			e = x0;	/* borrow it as a temp */
			x0 = x1;
			x1 = e;
			e = y0;
			y0 = y1;
			y1 = e;
		}
		my = y1 - y0;
		mx = abs(x1 - x0);
		e = mx - my;
		while (y0 <= y1) {
			__paint_pixel(g, x0, y0, color);
			gfx_draw_point(g, color);
			y0++;
			e += mx; 
			if (e > 0) {
				x0 += (x0 > x1) ? -1 : 1;
				e -= my;
			}
		}
	}
	return;
}

/*
 * __paint_rectangle
 *
 * This is the dead simple implementation of fill a rectangular
 * patch of the screen with a color. It assumes that w and h
 * are positive, non-zero values.
 */
static void
__paint_rectangle(GFX_CTX *g, int x, int y, int w, int h, GFX_COLOR c)
{
	int	y0 = y;

	for (y0 = y; y0 < y + h; y0++) {
		__paint_line(g, x, y0, x + (w - 1), y0, c);
	}
}

/*
 * __paint_quadrant
 *
 * quad is a bitfield that selects the quadrants to paint
 * fill is a boolean to specify filled.
 * r is the radius in pixels
 * The context is unchanged.
 */
static void
__paint_quadrant(GFX_CTX *g, int x0, int y0, int r, int quad, int fill, GFX_COLOR color)
{
	int	x, y;
	int i, err = 0;
	x = r;
	y = 0;
	while (x >= y) {
		if ((quad & 1) != 0) {
			if (fill) {
				for (i = 0; i <= x; i++) {
					__paint_pixel(g, x0 + i, y0 + y, color);
				}
				for (i = 0; i <= y; i++) {
					__paint_pixel(g, x0 + i, y0 + x, color);
				}
			}
			__paint_pixel(g, x0 + x, y0 + y, color);
			__paint_pixel(g, x0 + y, y0 + x, color);
		}
		if ((quad & 2) != 0) {
			if (fill) {
				for (i = 0; i <= y; i++) {
					__paint_pixel(g, (x0 - y) + i, y0 + x, color);
				}
				for (i = 0; i <= x; i++) {
					__paint_pixel(g, (x0 - x) + i, y0 + y, color);
				}
			}
			__paint_pixel(g, x0 - y, y0 + x, color);
			__paint_pixel(g, x0 - x, y0 + y, color);
		}
		if ((quad & 4) != 0) {
			if (fill) {
				for (i = 0; i <= x; i++) {
					__paint_pixel(g, (x0 - x) + i, y0 - y, color);
				}
				for (i = 0; i <= y; i++) {
					__paint_pixel(g, (x0 - y) + i, y0 - x, color);
				}
			}
			__paint_pixel(g, x0 - x, y0 - y, color);
			__paint_pixel(g, x0 - y, y0 - x, color);
		}
		if ((quad & 8) != 0) {
			if (fill) {
				for (i = 0; i <= y; i++) {
					__paint_pixel(g, x0 + i, y0 - x, color);
				}
				for (i = 0; i <= x; i++) {
					__paint_pixel(g, x0 + i, y0 - y, color);
				}
			}
			__paint_pixel(g, x0 + y, y0 - x, color);
			__paint_pixel(g, x0 + x, y0 - y, color);
		}
		if (err <= 0) {
			y++;
			err += 2 * y + 1;
		} else {
			x--;
			err -= 2*x + 1;
		}
	}
}

/*
 * __paint_triangle
 *
 * This is the code from devmaster.net/posts/6145/advanced-rasterization
 * It paints (filled) a triangle given by the three points.
 *
 * It has 33 local ints (so 132 bytes of additional stack space) and
 * a couple dozen instructions. It is pretty fast!
 */
static void
__paint_triangle(GFX_CTX *g, int x0, int y0, int x1, int y1, int x2, int y2, GFX_COLOR color)
{
	int x, y;
	int cross;

	/*
	 * This algorithm depends on the vertices being in CCW order. This
 	 * is also known as their "winding". We can compute if they are in
	 * order by computing the "Z" component of a cross product of two
 	 * vectors from a common point (assume z is 0). If it is postive
	 * they are, if not you need to swap the order of the second and
	 * third vertex.
	 * NB: This can overflow if you're using a "big" space (larger
 	 *     than 64K x 64K points.
	 *
	 * u1 = (x1 - x0), (y1 - y0), (z1 - z0)
	 * u2 = (x2 - x0), (y2 - y0), (z2 - z0)
	 * Compute the Z component of u1 X u2 = (u1x * u2y) - (u1y  * u2x)
	 *           ((x1 - x0) * (y2 - y0)) - ((y1 - y0) * (x2 - x0))
	 *
	 */
	cross = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
	if (cross == 0) {
		// they are co-linear so just draw a line
		__paint_line(g, min(x0, x1, x2), min(y0, y1, y2),
					 max(x0, x1, x2), max(y0, y1, y2), color);
		return;
	}

	/* 28.4 fixed point */
	const int Y1 = y0 << 4;
	const int Y2 = ((cross < 0) ? y1 : y2) << 4;
	const int Y3 = ((cross < 0) ? y2 : y1) << 4;

	const int X1 = x0 << 4;
	const int X2 = ((cross < 0) ? x1 : x2) << 4;
	const int X3 = ((cross < 0) ? x2 : x1) << 4;

	/* Deltas */
	const int DX12 = X1 - X2;
	const int DX23 = X2 - X3;
	const int DX31 = X3 - X1;

	const int DY12 = Y1 - Y2;
	const int DY23 = Y2 - Y3;
	const int DY31 = Y3 - Y1;

	/* Fixed point Deltas */
	const int FDX12 = DX12 << 4;
	const int FDX23 = DX23 << 4;
	const int FDX31 = DX31 << 4;

	const int FDY12 = DY12 << 4;
	const int FDY23 = DY23 << 4;
	const int FDY31 = DY31 << 4;

	/* Bounding rectangle */
	int minx = (min(X1, X2, X3) + 0xF) >> 4;
	int maxx = (max(X1, X2, X3) + 0xF) >> 4;
	int miny = (min(Y1, Y2, Y3) + 0xF) >> 4;
	int maxy = (max(Y1, Y2, Y3) + 0xF) >> 4;

	/* Half-edge constants */
	int C1 = DY12 * X1 - DX12 * Y1;
	int C2 = DY23 * X2 - DX23 * Y2;
	int C3 = DY31 * X3 - DX31 * Y3;

	/* Correct for fill convention */
	if ((DY12 < 0) || ((DY12 == 0) && (DX12 > 0))) {
		C1++;
	}
	if ((DY23 < 0) || ((DY23 == 0) && (DX23 > 0))) {
		C2++;
	}
	if ((DY31 < 0) || ((DY31 == 0) && (DX31 > 0))) {
		C3++;
	}

	int CY1 = C1 + DX12 * (miny << 4) - DY12 * (minx << 4);
	int CY2 = C2 + DX23 * (miny << 4) - DY23 * (minx << 4);
	int CY3 = C3 + DX31 * (miny << 4) - DY31 * (minx << 4);

	for (y = miny; y <= maxy; y++) {
		int t;
		int CX1 = CY1;
		int CX2 = CY2;
		int CX3 = CY3;
		for (x = minx; x <= maxx; x++) {
			if ((CX1 >= 0) && (CX2 >= 0) && (CX3 >= 0)) {
				__paint_pixel(g, x, y, color);
			} 
			CX1 -= FDY12;
			CX2 -= FDY23;
			CX3 -= FDY31;
		}
		CY1 += FDX12;
		CY2 += FDX23;
		CY3 += FDX31;
	}
}

/*
 * gfx_rotate( ... )
 *
 * This sets the display rotation by creating a
 * 2 x 2 transform that is used in the pixel
 * painting function to adjust x and y from
 * user space to display space.
 *
 * Transform is :
 *            +-------+--------+
 *   [X, Y]   | cos a | -sin a |
 *            +-------+--------+
 *        	  | sin a | cos a  |
 *            +-------+--------+
 */
void
gfx_rotate(GFX_CTX *g, float angle)
{
	g->cr.xfrm[0][0] = g->cr.xfrm[1][1] = cos((angle * M_PI) / 180.0);
	g->cr.xfrm[0][1] = -sin((angle * M_PI) / 180.0);
	g->cr.xfrm[1][0] = - g->cr.xfrm[0][1];
}

/*
 * gfx_set_rotation_origin( ... )
 *
 * Set the center of rotation for the display.
 * By default it is the center of the display
 * but at times you might want it somewhere else.
 *
 * NB: Because the display is an even number of lines
 * and pixels this usually works. But on bitmaps that
 * are an odd number of lines or pixels it can create
 * subtle artifacts.
 */
void
gfx_set_rotation_origin(GFX_CTX *g, int x, int y)
{
	g->cr.x = x;
	g->cr.y = y;
}

/*
 * Drop a single colored pixel at the current
 * location.
 *
 * I am reminded again of the absolute PITA and
 * importance of being really clear about 'screen' space
 * and 'user' space. And if rotation is about the center
 * or about the current X, Y. Clearly though X-invert and
 * optionally Y-invert should be post transform. Sigh I
 * feel like I'm backing into re-creating Cairo only not.
 * Still on the 32 bit architecture you can be more expressive
 * and that is useful.
 *
 */
void
gfx_draw_point(GFX_CTX *gfx, GFX_COLOR color) {
	__paint_pixel(gfx, gfx->cx, gfx->cy, color);
}

/*
 * gfx_move_to( ... )
 *
 *	Set the 'current' location to a specific place
 * in user space. 
 */
void
gfx_move_to(GFX_CTX *g, int x, int y)
{
	g->cx = x;
	g->cy = y;
}

/*
 * gfx_move( ... )
 *
 * Move by a relative amount from the 'current' position.
 */
void
gfx_move(GFX_CTX *g, int x, int y)
{
	g->cx += x;
	g->cy += y;
}

/*
 * gfx_draw_line( ... )
 *
 * Draw a line from the 'current' position to the
 * destination postion which is relative to the 
 * current position.
 */
void
gfx_draw_line(GFX_CTX *g, int x, int y, GFX_COLOR color)
{
	__paint_line(g, g->cx, g->cy, g->cx + x, g->cy + y, color);
	g->cx += x;
	g->cy += y;
}

/*
 * gfx_draw_circle( ... )
 *
 * Draw a circle at the current location of radius
 * 'r'.
 */
void
gfx_draw_circle(GFX_CTX *g, int r, GFX_COLOR color) {
	if (r <= 0) {
		return;
	}
	__paint_quadrant(g, g->cx, g->cy, r, 0xf, 0, color);
}

/*
 * gfx_fill_circle( ... )
 *
 * Draw a filled circle at the current location of
 * radius 'r'.
 */
void
gfx_fill_circle(GFX_CTX *g, int r, GFX_COLOR color) {
	if (r <= 0) {
		return;
	}
	__paint_quadrant(g, g->cx, g->cy, r, 0xf, 1, color);
}

/*
 * gfx_draw_triangle( ... )
 * 
 * Draw a triangle, the starting point is the current 
 * location which does not change. 
 */
void
gfx_draw_triangle(GFX_CTX *g, int x0, int y0, int x1, int y1, GFX_COLOR color)
{
	__paint_line(g, g->cx, g->cy, g->cx + x0, g->cy + y0, color);
	__paint_line(g, g->cx, g->cy, g->cx + x1, g->cy + y1, color);
	__paint_line(g, g->cx + x0, g->cy + y0, g->cx + x1, g->cy + y1, color);
}

/*
 * gfx_fill_triangle( ... )
 * 
 * Draw a filled triangle, the starting point is the current 
 * location which does not change. 
 */
void
gfx_fill_triangle(GFX_CTX *g, int x0, int y0, int x1, int y1, GFX_COLOR color)
{
	__paint_triangle(g, g->cx, g->cy, 
						g->cx + x0, g->cy + y0,
						g->cx + x1, g->cy + y1,  color);
}

/*
 * gfx_draw_rectangle( ... )
 *
 * Draw a rectangle at the current location. Note that
 * by default it draws to the right and down but if you
 * use a negative width it will draw left, and a negative
 * height and it will draw up.
 */
void
gfx_draw_rectangle(GFX_CTX *g, int w, int h, GFX_COLOR color)
{
	int x0, y0, x1, y1;

	if ((w * h) == 0) {
		return;
	}

	if (w < 0) {
		x0 = g->cx + w + 1; x1 = g->cx;
	} else {
		x0 = g->cx; x1 = g->cx + (w - 1);
	}
	if (h < 0) {
		y0 = g->cy + h + 1; y1 = g->cy;
	} else {
		y0 = g->cy; y1 = g->cy + (h -1);
	}

	__paint_line(g, x0, y0, x1, y0, color);
	__paint_line(g, x1, y0, x1, y1, color);
	__paint_line(g, x1, y1, x0, y1, color);
	__paint_line(g, x0, y1, x0, y0, color);
}

/*
 * gfx_fill_rectangle( ... )
 *
 * Draw a filled rectangle at the current location. Note that
 * by default it draws to the right and down but if you
 * use a negative width it will draw left, and a negative
 * height and it will draw up.
 * Draw a filled rectangle
 */
void
gfx_fill_rectangle(GFX_CTX *g, int w, int h, GFX_COLOR color)
{
	int x0, y0, y1;

	if ((w * h) == 0) {
		return;
	}

	if (w < 0) {
		x0 = g->cx + w + 1;
		w = -w;
	} else {
		x0 = g->cx;
	}
	if (h < 0) {
		y0 = g->cy + h + 1; y1 = g->cy;
		h = -h;
	} else {
		y0 = g->cy;
	}
	__paint_rectangle(g, x0, y0, w, h, color);
}

/*
 * gfx_fill_screen( ... )
 *
 * Clear the screen to a particular color. 
 * TODO: Add optional call back to do this efficently.
 */
void
gfx_fill_screen(GFX_CTX *g, GFX_COLOR color)
{
	float t[2][2];
	memcpy(t, g->cr.xfrm, sizeof(t)); /* backup rotation transform */
	g->cr.xfrm[0][0] = 1;
	g->cr.xfrm[1][1] = 1;
	g->cr.xfrm[0][1] = 0;
	g->cr.xfrm[1][0] = 0;
	__paint_rectangle(g, 0, 0, g->w, g->h, color);
	memcpy(g->cr.xfrm, t, sizeof(t)); /* restore rotation transform */
}

/*
 * gfx_draw_rounded_rectangle( ... )
 *
 * Draw a rectangle with rounded corners. Internally this provides
 * a good test if the quadrant drawing code and line drawing code
 * works together without gaps etc.
 */

#define TEST_C1 (GFX_COLOR){ .raw=(uint32_t)'1'}
#define TEST_C2 (GFX_COLOR){ .raw=(uint32_t)'2'}

void
gfx_draw_rounded_rectangle(GFX_CTX *g, int w, int h, int r, GFX_COLOR color)
{
	/* error check */
	if ((r > (w / 2)) ||	/* radius is larger than 1/2 width */
		(r > (h/2)) ||		/* radius is larger than 1/2 height */
		((((h - 2 * r) == 0)) && ((w - 2 * r) == 0)) ||	/* its a circle */
		(r <= 0))			/* if 0 they want a regular rectangle */
	{
		return; 
	}

	/* oddly formatted to be easier to read on wide screen */
	__paint_quadrant(g, g->cx + r,       g->cy + r,       r, 4, 0, color); /* top left */
	__paint_quadrant(g, g->cx + (w - r - 1), g->cy + r,       r, 8, 0, color); /* top right */
	__paint_quadrant(g, g->cx + r,       g->cy + (h - r - 1), r, 2, 0, color); /* bottom left */
	__paint_quadrant(g, g->cx + (w - r - 1), g->cy + (h - r - 1), r, 1, 0, color); /* bottom right */
	/* oddly formatted to be easier to read on wide screen */
	__paint_line(g, g->cx + r + 1, g->cy,         g->cx + (w - r - 2), g->cy,               color); /* top */
	__paint_line(g, g->cx,         g->cy + r + 1, g->cx,               g->cy + (h - r - 2), color); /* left */
	__paint_line(g, g->cx + w - 1, g->cy + r + 1, g->cx + w - 1,       g->cy + (h - r - 2), color); /* right */
	__paint_line(g, g->cx + r + 1, g->cy + h - 1, g->cx + (w - r - 2), g->cy + h - 1,       color); /* bottom */
}

/*
 * gfx_fill_rounded_rectangle( ... )
 *
 * Draw a rectangle with rounded corners. Internally this provides
 * a good test if the filling code works together without gaps etc.
 */
void
gfx_fill_rounded_rectangle(GFX_CTX *g, int w, int h, int r, GFX_COLOR color)
{
	/* error check */
	if ((r > (w / 2)) ||	/* radius is larger than 1/2 width */
		(r > (h/2)) ||		/* radius is larger than 1/2 height */
		((((h - 2 * r) == 0)) && ((w - 2 * r) == 0)) ||	/* its a circle */
		(r <= 0))			/* if 0 they want a regular rectangle */
	{
		return; 
	}

	/* oddly formatted to be easier to read on wide screen */
	__paint_quadrant(g, g->cx + r,       g->cy + r,       r, 4, 1, color);
	__paint_quadrant(g, g->cx + (w - r - 1), g->cy + r,       r, 8, 1, color);
	__paint_quadrant(g, g->cx + r,       g->cy + (h - r - 1), r, 2, 1, color);
	__paint_quadrant(g, g->cx + (w - r - 1), g->cy + (h - r - 1), r, 1, 1, color);
	/*
	 * TODO Fix this to use paint_rectangle
	 * Note: There are three 'internal' rectangles to be filled;
	 *     - one is between the top and bottom left quadrants,
	 *     - one is between all four quadrants,
	 *     - and one is between thr top and bottom right quadrants.
	 */
	if ((h - 2 * r) > 0) {
		__paint_triangle(g, g->cx,     g->cy + r,
							g->cx + r, g->cy + r,
							g->cx + r, g->cy + (h - r - 1), color);
		__paint_triangle(g, g->cx,     g->cy + r,
							g->cx,     g->cy + (h - r - 1),
							g->cx + r, g->cy + (h - r - 1), color);
		__paint_triangle(g, g->cx + (w - r - 1), g->cy + r,
							g->cx + (w - 1),     g->cy + r,
							g->cx + (w - 1),     g->cy + (h - r - 1), color);
		__paint_triangle(g, g->cx + (w - r - 1), g->cy + r,
							g->cx + (w - r - 1), g->cy + (h - r - 1),
							g->cx + (w - 1),           g->cy + (h - r - 1), color);
	}

	if ((w - 2 * r) > 0) {
		__paint_triangle(g, g->cx + r + 1,       g->cy,
							g->cx + (w - r - 2), g->cy,
							g->cx + (w - r - 2), g->cy + (h - 1), color);
		__paint_triangle(g, g->cx + r + 1,       g->cy,
							g->cx + r + 1,       g->cy + (h - 1),
							g->cx + (w - r - 2), g->cy + (h - 1), color);
	}

}

#if 0
/*
 * XXX this is not useful code? XXX
 * used to be 'draw bitmap'
 * This code puts a text glyph onto the screen. It defines by its
 * function how rotation works and cursor placement.
 * NB: Global discussion about baseline vs top line vs bottom line
 *	   as the 'origin' point for glyphs.
 */
static void
__paint_glyph(GFX_CTX *g,
	int x, int y, const uint8_t *bitmap, int w, int h, GFX_COLOR color)
{
	int i, j, byteWidth = (w + 7) / 8;
	int tx, ty;

	for(j = 0; j < h; j++) {
		for(i = 0; i < w; i++ ) {
			if(*(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
				/* rotate */
				tx = i * g->text.cr.xfrm[0][0] + i * g->text.cr.xfrm[1][0];
				ty = j * g->text.cr.xfrm[0][1] + j * g->text.cr.xfrm[1][1];
				__paint_pixel(g, x+tx, y+ty, color);
			}
		}
  	}
}
#endif

/*
 * gfx_set_text_rotation( ... )
 *
 * This sets the angle at which text is rendered into the screen. The
 * rotation 'origin' is always the character's origin and the cursor
 * advances along this angled line.
 */
void
gfx_set_text_rotation(GFX_CTX *g, float angle)
{
	g->text.cr.angle = angle;
	g->text.cr.xfrm[0][0] = g->text.cr.xfrm[1][1] = cos((angle * M_PI) / 180.0);
	g->text.cr.xfrm[0][1] = -sin((angle * M_PI) / 180.0);
	g->text.cr.xfrm[1][0] = - g->text.cr.xfrm[0][1];
}

/*
 * gfx_set_text_cursor( ... )
 *
 * This is where the next character will be rendered. 
 * TODO: Discussion on topline vs baseline vs bottomline.
 */
void
gfx_set_text_cursor(GFX_CTX *g, int x, int y)
{
	g->text.cx = x;
	g->text.cy = y;
}

/*
 * gfx_set_text_color( ... )
 *
 * Set the foreground and background color of the text
 * rendering functions. Note if foreground and background
 * are set to the same color, then it is as if the background
 * color is 'transparent' (doesn't get rendered).
 */
void
gfx_set_text_color(GFX_CTX *g, GFX_COLOR fg, GFX_COLOR bg)
{
	g->text.fg = fg;
	g->text.bg = bg;
}

/*
 * gfx_set_text_size( ... )
 *
 * Set a magnification value for the text
 */
void
gfx_set_text_size(GFX_CTX *g, int size)
{
	g->text.magnify = size;
}

/*
 * gfx_get_text_height( ... )
 *
 * Return height of characters if set.
 */
int
gfx_get_text_height(GFX_CTX *g)
{
	if (g->glyphs != NULL) {
		return g->glyphs->height;
	}
	return 0;
}

/*
 * gfx_get_text_baseline( ... )
 *
 * Returns the 'baseline' for the characters
 */
int
gfx_get_text_baseline(GFX_CTX *g)
{
	if (g->glyphs != NULL) {
		return g->glyphs->baseline;
	}
	return 0;
}

/*
 * gfx_get_text_width( ... )
 *
 * Return width of characters if set.
 */
int
gfx_get_text_width(GFX_CTX *g)
{
	if (g->glyphs != NULL) {
		return g->glyphs->width;
	}
	return 0;
}

/*
 * gfx_get_string_width( ... )
 *
 * Return the length of a string in pixels based
 * on the current font settings.
 */
int
gfx_get_string_width(GFX_CTX *g, char *str)
{
	int w;
	if ((str == NULL) || (g->glyphs == NULL)) {
		return 0;
	}
	w = strlen(str);
	/* fixed width fonts are *simple* remember that */
	return w * g->text.magnify * g->glyphs->width;
}

/*
 * gfx_get_font( ... )
 *
 * Returns the size of the currently set font
 */
GFX_FONT
gfx_get_font(GFX_CTX *g)
{
	return g->text.font;
}

GFX_FONT
gfx_set_font(GFX_CTX *g, GFX_FONT font)
{
	GFX_FONT	old = g->text.font;
	g->text.font = font;
	/* Always set small by default */
	switch (font) {
		case GFX_FONT_TINY:
			g->glyphs = &tiny_font;
			break;	
		case GFX_FONT_LARGE:
			g->glyphs = &large_font;
			break;
		default:
			g->glyphs = &small_font;
			break;
	}
	return old;
}

/*
 * __paint_glyph
 *
 * Take a glyph and paint it to the bitmap.
 * This does the heavy lifting of putting a font glyph associated
 * with an ASCII value.
 */
static void
__paint_glyph(GFX_CTX *g, int x, int y, uint8_t c, GFX_COLOR fg, GFX_COLOR bg) {
	const uint8_t	*glyph;
	GFX_FONT_GLYPHS	*font;
	int i, k;
	int	tm = g->text.magnify; /* text magnify */
	uint8_t descender, bit;
	int dx, dy, tx, ty;

	if ((int) c  >  g->glyphs->chars) {
		return; // no glyph for this character.
	}
	if (g->glyphs == NULL) {
		return;
	}
	font = g->glyphs;

	glyph = font->raw + (font->size * c);
	descender = ((*glyph & 0x80) != 0);
	for (k = 0; k < font->size; k++) {
		for (i = 0; i < 7; i++) {
			bit = ((*glyph & (0x40 >> i)) != 0);
			dx = ((font->byrow) ? (i) : (k));
			dy = ((font->byrow) ? (k) : (i)) + (descender * 3) - font->baseline;
			dx *= tm;
			dy *= tm;
			/* apply text rotation */
			tx = dx * g->text.cr.xfrm[0][0] + dy * g->text.cr.xfrm[1][0];
			ty = dx * g->text.cr.xfrm[0][1] + dy * g->text.cr.xfrm[1][1];
			if (bit) {
				if (tm > 1) {
					__paint_triangle(g, x + tx, y + ty,
										x + tx + tm, y + ty,
										x + tx + tm, y + ty + tm, fg);
					__paint_triangle(g, x + tx, y + ty,
										x + tx, y + ty + tm,
										x + tx + tm, y + ty + tm, fg);
//					gfx_fillRect(x + dx, y + dy, size, size, fg);
				} else {
					__paint_pixel(g, x + tx, y + ty, fg);
				}
			} else if (! SAME_COLOR(bg, fg)) {
				if (tm > 1) {
					__paint_triangle(g, x + tx, y + ty,
										x + tx + tm, y + ty,
										x + tx + tm, y + ty + tm, bg);
					__paint_triangle(g, x + tx, y + ty,
										x + tx, y + ty + tm,
										x + tx + tm, y + ty + tm, bg);
//					gfx_fillRect(x + dx, y + dy, size, size, bg);
				} else {
					__paint_pixel(g, x + tx, y + ty, bg);
				}
			}
		}
		glyph++;
	}
}

/*
 * gfx_draw_glyph( ... )
 *
 * Draw the glyph in the current font for the given
 * code point.
 * Notes:
 *		- This picks up rotation and size from 
 *		  the current context. 
 *		- The 'position' of the cursor is unchanged
 *		  (both Graphics and Text)
 */
void
gfx_draw_glyph(GFX_CTX *g, uint8_t cp, GFX_COLOR fg, GFX_COLOR bg)
{
	__paint_glyph(g, g->text.cx, g->text.cy, cp, fg, bg);
}

/*
 * gfx_putc( ... )
 *
 * This draws the character in the current font.
 * Notes:
 *		- This picks up rotation and size from 
 *		  the current context. 
 *		- The 'position' of the text cursor is moved
 *		  ahead based on text rotation by one
 *		  character width.
 *		- Character color (fg, bg) is picked up from
 *		  the text context. (gfx_set_text_color(...))
 */
void
gfx_putc(GFX_CTX *g, char c)
{
	int tx, ty;
	__paint_glyph(g, g->text.cx, g->text.cy, c, g->text.fg, g->text.bg);
	g->text.cx += g->glyphs->width * g->text.magnify * g->text.cr.xfrm[0][0];
	g->text.cy += g->glyphs->width * g->text.magnify * g->text.cr.xfrm[0][1];
}

/*
 * gfx_puts( ... )
 *
 * This then puts a null terminated string on the screen
 * in the current font.
 * Notes:
 *		- This picks up rotation and size from 
 *		  the current context. 
 *		- The 'position' of the text cursor is moved
 *		  ahead based on text rotation by one
 *		  character width.
 *		- Character color (fg, bg) is picked up from
 *		  the text context. (gfx_set_text_color(...))
 */
void
gfx_puts(GFX_CTX *g, char *s)
{
	while (*s != 0) {
		gfx_putc(g, *s);
		s++;
	}
}

/*
 * gfx_set_mirrored( ... )
 *
 * Sets a mirroring of the x axis so that it
 * 'looks correct' in a mirror. I have used this
 * when making a heads up display.
 */
void
gfx_set_mirrored(GFX_CTX *g, int m)
{
	if (m) {
		g->flags |= GFX_DISPLAY_INVERT;
	} else {
		g->flags &= ~GFX_DISPLAY_INVERT;
	}
}

int
gfx_get_mirrored(GFX_CTX *g)
{
	return ((g->flags & GFX_DISPLAY_INVERT) != 0);
}
