# Software Rasterizer

A 2D and 3D graphics software rasterizer implemented in C. Includes SDL backend to abstract OS-specifics (window management, input, etc).

This is basically a "for fun" toy project of mine. Probably not too noteworthy and definitely has been done better before by people far smarter then me!

**In early development! Pre-alpha quality code!**

2D features:

* 24 bit (RGB) and 32 bit (RGBA) surface support
* drawing primitives: pixel, line, rectangle (outline and filled)
* sprite drawing: solid, transparent, blending, color tinting
* clipping support on all drawing operations as well as "fast" variants which skip clipping
* image -> surface loading via stb_image
* TTF -> sprite font texture atlas via stb_truetype (TODO)

3D features:

* TODO


