Notes on V2
-----------

As I uncover things and remind myself of previous issues I'm collecting
them here in the notes document.

So a big goal of the V2 rewrite was to give relative drawing because it
is so much easier to write 
`moveto(100, 100); draw(100,0); draw(0,100); draw(-100,0); draw(0, -100);`
than to use absolute co-ordinates all the time.

But there is this rotation question, does it happen in display space or
user space? And keeping display space and user space both separate and
coherent so that one can always transform from one to the other without
unexpected results.

Ground truth in the context structure:
        `x, y` -- this is the 'current' draw point, it is in *user* space.
`width, height` -- this is the size of the underlying thing.
       `rx, ry` -- this is the 'center' of the current rotation.
          `rot` -- this is the current user rotation in *degrees*
     `void *fb` -- this is what draw_point needs to do its magic.
   `*user_draw` -- this will take co-ordinates in bitmap space (x, y)


The `*fb` is probably a `dma2d_bitmap` for most things.
The `user_draw` function will translate between a `GFX_COLOR` to the DMA2D
form needed by the bitmap.

Filled Rects are decomposed into triangles.
Filled Rounded Rects are decomposed into triangles and filled arcs.

We use bresenham for lines but sin/cos for arcs. Need a fast filled arc function.

This is what I see as a decomposition

LCD functions -- `lcd_init`, `lcd_flip`, `lcd_blit`
DMA2D functions	-- `dma2d_clear`, `dma2d_blit`

Typical usage
```
	GFX *g_lcd	= lcd_init();
	x = gfx_width(g_lcd);
	y = gfx_height(g_lcd);
	for (i = 0; i < 100; i++) {
		gfx_moveto(g_lcd, x+i, y-i);
		gfx_lineto(g_lcd, x-i, y+i);
	}
	lcd_flip(0);
```

