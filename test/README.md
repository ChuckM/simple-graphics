#Various Tests on the library

This directory contains a variety of tests that I've written to quickly test out the library.
They share a common framework, they make a "text" framebuffer (if you were drawing with
ASCII text for example) and the "colors" are generally character symbols so `C_DOT` is a 
period (.) and `C_STAR` is an asterisk (*). 

Then the output can be displayed by either editing it (you may want to disable line wrap
if your screen isn't wide enough) or just showing it in a terminal window.

##basic
This test is basic function tests, it draws a few shapes and a couple of messages.

##chartst
This test exercises the character (text) functions, it rotates text and scales it
based on the argument passed.

##large-font
This dumps out the "large" (9 x 12) font (all 256 characters of it) and is good for
debugging font issues.

##plot
This doesn't work.

##rectest
This tests the rectangle and rounded rectangle functions.

##rotation
I had a bug with rotation I was trying to fix and I used this test to exercise all of the
rotation options.

##rottest
This was another rotation test.

##small-font
This dumps the small font for debugging glyphs.

##tiny-font
This dumps the tiny font for debugging glyphs.

##triangle-fill
Since I use a fairly complex triangle fill I wrote some tests to validate the various
edge cases.

##tritest
This is a another triangle test.

##vptest
This tests the "viewport" function.

