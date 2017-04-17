/* 
 * Simple Graphics
 *
 * Copyright (c) 2014-2017 Charles McManis, all rights reserved.
 *
 * Version 2
 *
 * Basic prototype definitions and flag #defines for the simple
 * graphics routines.
 *
 */
#ifndef _SIMPLE_GFX_H
#define _SIMPLE_GFX_H
#include <stdint.h>


#define GFX_DISPLAY_INVERT	8

typedef union __gfx_color {
	struct {
		uint32_t	b:8;
		uint32_t	g:8;
		uint32_t	r:8;
		uint32_t	a:8;
	} c;
	uint32_t	raw;
} GFX_COLOR;

#define COLOR(red,grn,blu)	(GFX_COLOR){.c = {blu, grn, red, 0xff}}
#define SAME_COLOR(c1, c2)	((c1.c.r == c2.c.r) && \
				 (c1.c.g == c2.c.g) && \
				 (c1.c.b == c2.c.b))

typedef enum { GFX_ROT_0, GFX_ROT_90, GFX_ROT_180, GFX_ROT_270 } GFX_ROTATION;
typedef enum { GFX_FONT_TINY, GFX_FONT_SMALL, GFX_FONT_LARGE } GFX_FONT;

/*
 * 256 glyph font structure, nominally ASCII or ISO-LATIN1. Sorry but I'm not
 * going to do Unicode on an embedded micro's LCD display just yet.
 */
typedef struct {
	const uint8_t	*raw;	/* raw font data */
	int16_t			chars;	/* number of characters in font */
	int16_t			size;	/* bytes per glyph */
	uint8_t			byrow;	/* row oriented or column oriented flag */
	uint8_t			width;	/* spacing between characters. */
	uint8_t			height;	/* spacing between rows of characters. */
	uint8_t			baseline;	/* # of lines above 'y' to the top of glyph */
} GFX_FONT_GLYPHS;

/*
 * state of rotation of the display
 *
 */
typedef struct {
	float	angle;		/* rotation angle */
	int		x, y;		/* origin of the rotation */
	float	xfrm[2][2];	/* 2 X 2 matrix */
} GFX_ROTATION_STATE;

/*
 * This maintains the 'state' of the graphics context
 */
typedef struct {
	void (*drawpixel)(void *, int, int, GFX_COLOR);	/* user supplied pixel writer */
	uint16_t	w, h;				/* dimensions of drawable space */
	int16_t cx, cy;					/* current x and y in *user* space */
	uint16_t	flags;				/* State flags for library */
	GFX_ROTATION_STATE	cr;
	void *fb;						/* user supplied frame buffer pointer */
	struct {
		int16_t cx, cy;				/* Current "cursor" X/Y location */
		GFX_COLOR bg, fg;			/* Background and foreground colors */
		GFX_FONT font;				/* Text "font" */
		int magnify;				/* Text "magnification" */
		GFX_ROTATION_STATE cr;		/* Text rotation direction */
	} text;
	GFX_FONT_GLYPHS *glyphs;		/* Current font in use */
} GFX_CTX;


GFX_CTX *gfx_init(void (*draw)(void *, int, int, GFX_COLOR),
				int width, int height, GFX_FONT size, void *fb);

void gfx_move_to(GFX_CTX *g, int x, int y);
void gfx_move(GFX_CTX *g, int x, int y);

void gfx_draw_point(GFX_CTX *g, GFX_COLOR color);
void gfx_draw_line(GFX_CTX *g, int x, int y, GFX_COLOR color);

/* set rotation [0 degress by default] */
void gfx_rotate(GFX_CTX *g, float angle);
/* set rotation axis [w/2, h/2] by default */
void gfx_set_rotation_origin(GFX_CTX *g, int x, int y);

void gfx_draw_rectangle(GFX_CTX *g, int w, int h, GFX_COLOR color);
void gfx_fill_rectangle(GFX_CTX *g, int w, int h, GFX_COLOR color);
void gfx_draw_rounded_rectangle(GFX_CTX *g, int w, int h, int r, GFX_COLOR color);
void gfx_fill_rounded_rectangle(GFX_CTX *g, int w, int h, int r, GFX_COLOR color);

void gfx_fill_screen(GFX_CTX *g, GFX_COLOR color);

void gfx_draw_circle(GFX_CTX *g, int r, GFX_COLOR color);
void gfx_fill_circle(GFX_CTX *g, int r, GFX_COLOR color);

void gfx_draw_triangle(GFX_CTX *g, int ax, int ay, int bx, int by, GFX_COLOR c);
void gfx_fill_triangle(GFX_CTX *g, int ax, int ay, int bx, int by, GFX_COLOR c);


/*
 * Text handling functions.
 * NB: This is 'simple' graphics so each font has at most 256
 *     glyphs.
 */
void gfx_set_text_rotation(GFX_CTX *g, float angle);
void gfx_set_text_color(GFX_CTX *g, GFX_COLOR fg, GFX_COLOR bg);
void gfx_set_text_cursor(GFX_CTX *g, int x, int y);
void gfx_set_text_size(GFX_CTX *g, int sz);
int	 gfx_get_text_height(GFX_CTX *g);
int	 gfx_get_text_width(GFX_CTX *g);

int  gfx_get_string_width(GFX_CTX *g, char *str);
void gfx_putc(GFX_CTX *g, char c);
void gfx_puts(GFX_CTX *g, char *s);
GFX_FONT gfx_get_font(GFX_CTX *g);
GFX_FONT gfx_set_font(GFX_CTX *g, GFX_FONT size);
void gfx_draw_glyph(GFX_CTX *g, uint8_t c, GFX_COLOR fg, GFX_COLOR bg);
void gfx_set_font_glyphs(GFX_CTX *g, GFX_FONT_GLYPHS *glyph);

/* syntactic sugar really */
#define gfx_get_width(g)	g->width
#define gfx_get_height(g)	g->height

/* XXX fix these to be GFX_COLORS */

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

#endif  /* SIMPLE_GFX */
