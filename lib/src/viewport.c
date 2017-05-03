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
#include <stdlib.h>
#include <string.h> /* for memset */
#include <gfx.h>

/* Implement the mapping from viewport to display space */
#define MAP_X(vp, inX)	(int)((((inX) + (vp)->ox) / (vp)->sx) + (vp->x))
/* flip Y from 'natural' co-ordinates (+Y goes 'up') to 'display' (+y goes 'down') */
#define MAP_Y(vp, inY)	(int)((vp->h) - (((inY) + (vp)->oy) / (vp)->sy) + (vp->y))

#if 0
typedef struct {
	GFX_CTX		*g;
	float		sx, sy;		/* X scale and Y scale */
	float		ox, oy;		/* offset X and offset Y */
	int			x, y, w, h;	/* box on the screen to use */
} GFX_VIEW;
#endif

GFX_VIEW __local_view;


/*
 * gfx_viewport( ... )
 *
 * Create a transform that will scale between floating
 * point (x,y) co-ordinates into a region on the display
 * screen.
 */
GFX_VIEW *
gfx_viewport(GFX_CTX *g, int x, int y, int w, int h, 
	float min_x, float min_y, float max_x, float max_y)
{
	GFX_VIEW *res = &__local_view;
	memset(res, 0, sizeof(GFX_VIEW));
	res->g = g;
	res->x = x;
	res->y = y;
	res->w = w;
	res->h = h;
	
	res->sx = (max_x - min_x) / (float) w;
	res->sy = (max_y - min_y) / (float) h;
	res->ox = -min_x;
	res->oy = -min_y;
	return &__local_view;
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
	gfx_move_to(v->g, MAP_X(v, x0), MAP_Y(v, y0));
	gfx_draw_line_to(v->g, MAP_X(v, x1), MAP_Y(v, y1), c);
}
