/* 
 * Simple Graphics
 *
 * Copyright (c) 2014-2015 Charles McManis, all rights reserved.
 *
 * This source code is licensed under a Creative Commons 4.0 
 * International Public license. 
 *
 * See: http://creativecommons.org/licenses/by/4.0/legalcode for
 * details.
 * 
 * Basic prototype definitions and flag #defines for the simple
 * graphics routines.
 *
 */
#ifndef _SIMPLE_GFX_H
#define _SIMPLE_GFX_H
#include <stdint.h>


#define GFX_FONT_LARGE		1
#define GFX_FONT_SMALL		2
#define GFX_TEXT_WRAP		4
#define GFX_DISPLAY_INVERT	8
#define GFX_FONT_TINY		0x10

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

enum gfx_rotate { GFX_ROT_0, GFX_ROT_90, GFX_ROT_180, GFX_ROT_270 };

void gfx_drawPixel(int x, int y, GFX_COLOR color);
void gfx_drawLine(int x0, int y0, int x1, int y1, GFX_COLOR color);
void gfx_drawFastVLine(int x, int y, int h, GFX_COLOR color);
void gfx_drawFastHLine(int x, int y, int w, GFX_COLOR color);
void gfx_drawRect(int x, int y, int w, int h, GFX_COLOR color);
void gfx_fillRect(int x, int y, int w, int h, GFX_COLOR color);
void gfx_fillScreen(GFX_COLOR color);

void gfx_drawCircle(int x0, int y0, int r, GFX_COLOR color);
void gfx_drawCircleHelper(int x0, int y0, int r, uint8_t cornername,
      GFX_COLOR color);
void gfx_fillCircle(int x0, int y0, int r, GFX_COLOR color);
void gfx_init(void (*draw)(int, int, GFX_COLOR), int, int, int font_size);

void gfx_fillCircleHelper(int x0, int y0, int r, uint8_t cornername,
      int delta, GFX_COLOR color);
void gfx_drawTriangle(int x0, int y0, int x1, int y1,
      int x2, int y2, GFX_COLOR color);
void gfx_fillTriangle(int x0, int y0, int x1, int y1,
      int x2, int y2, GFX_COLOR color);
void gfx_drawRoundRect(int x0, int y0, int w, int h,
      int radius, GFX_COLOR color);
void gfx_fillRoundRect(int x0, int y0, int w, int h,
      int radius, GFX_COLOR color);
void gfx_drawBitmap(int x, int y, const uint8_t *bitmap,
      int w, int h, GFX_COLOR color);
void gfx_drawChar(int x, int y, unsigned char c, GFX_COLOR fg,
      GFX_COLOR bg, int size, enum gfx_rotate rotation);
void gfx_setTextRotate(enum gfx_rotate r);
enum gfx_rotate gfx_getTextRotate(void);
void gfx_setCursor(int x, int y);
void gfx_setTextColor(GFX_COLOR c, GFX_COLOR bg);
void gfx_setTextSize(int s);
void gfx_setTextWrap(int w);
int	gfx_getTextWidth(void);
int gfx_getTextHeight(void);
void gfx_setRotation(enum gfx_rotate r);
void gfx_setMirrored(int m);
int gfx_getMirrored(void);
void gfx_puts(unsigned char *);
void gfx_putc(unsigned char);
int gfx_write(unsigned char );

int gfx_height(void);
int gfx_width(void);
enum gfx_rotate gfx_getTextRotation(void);
void gfx_setTextRotation(enum gfx_rotate);

int gfx_getFont(void);
void gfx_setFont(int font);
enum gfx_rotate gfx_getRotation(void);

/*
 * ASCII Font structure
 */
struct gfx_font {
	const uint8_t	*raw;	/* raw font data */
	int16_t			chars;	/* number of characters in font */
	int16_t			size;	/* bytes per glyph */
	uint8_t			byrow;	/* row oriented or column oriented flag */
	uint8_t			width;	/* spacing between characters. */
	uint8_t			height;	/* spacing between rows of characters. */
	uint8_t			baseline;	/* # of lines above 'y' to the top of glyph */
};

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
