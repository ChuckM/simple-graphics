/*
 * view.c -- View port handling
 *
 * Copyright (c) 2017, Chuck McManis, All Rights Reserved.
 *
 * <license>
 *
 * <warranty>
 *
 * This code defines a "viewport" abstraction. Basically if you want
 * to plot a function in cartesian space, you want to do it using the
 * co-ordinates of the function. So it creates a scaling transform to
 * convert a set of minima and maxima in the X and Y directions, into
 * a scaling factor in the X and Y directions so that drawing a line
 * in 'viewport' space shows up correctly in 'display' space.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for memset */
#include <gfx.h>

/*
 * return a value between the minimum and maximum value
 */
static int
minmax(int minimum, int a, int maximum)
{
	if ((a >= minimum) && (a < maximum)) {
		return a;
	} else if (a < minimum) {
		return minimum;
	}
	return maximum;
}

/*
 * gfx_viewport( ... )
 *
 * Create a transform that will scale between floating
 * point (x,y) co-ordinates into a region on the display
 * screen.
 */
GFX_VIEW *
gfx_viewport(GFX_VIEW *vprt, GFX_CTX *g, int x, int y, int w, int h, 
	float min_x, float min_y, float max_x, float max_y)
{
	GFX_VIEW *res = (vprt == NULL) ? malloc(sizeof(GFX_VIEW)) : vprt;
	memset(res, 0, sizeof(GFX_VIEW));
	res->g = g;
	res->x = x;
	res->y = y;
	res->w = w;
	res->h = h;
	
	/* Set scale for X and scale for Y */
	res->sx = (float) w / (max_x - min_x);
	res->sy = (float) h / (max_y - min_y);
	/* Offsets to move pixels into a 0 - (n+m) space from a -n to +m space */
	res->min_x = min_x;
	res->min_y = min_y;
	res->max_x = max_x;
	res->max_y = max_y;
	return res;
}

static void
transform(GFX_VIEW *v, float x_in, float y_in, int *x_out, int *y_out)
{
	x_in = (x_in - v->min_x) * v->sx + v->x;
	/* flip Y co-ordinate "+y is up in viewport, down in display" */
	y_in = (v->h + v->y) - ((y_in - v->min_y) * v->sy);
	*x_out = minmax(v->x, x_in, v->x + v->w);
	*y_out = minmax(v->y, y_in, v->y + v->h);
}

/*
 * vp_plot( ... )
 *
 * Draw a line from [x0, y0] to [x1, y1] in the viewport space, which
 * is transformed into a line draw in display space.
 */
void
vp_plot(GFX_VIEW *v, float x0, float y0, float x1, float y1, GFX_COLOR c)
{
	int	x, y;

	transform(v, x0, y0, &x, &y);
	gfx_move_to(v->g, x, y);

	transform(v, x1, y1, &x, &y);
	gfx_draw_line_to(v->g, x, y, c);
}
