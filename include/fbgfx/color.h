#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../types.h"

typedef uint32_t COLOR;
typedef uint8_t COLOR_COMPONENT;

static const COLOR_COMPONENT TRANSPARENT = 0;
static const COLOR_COMPONENT OPAQUE = 255;

static inline COLOR_COMPONENT color_get_a(COLOR color) {
	return (color & 0xff000000) >> 24;
}

static inline COLOR_COMPONENT color_get_r(COLOR color) {
	return (color & 0x00ff0000) >> 16;
}

static inline COLOR_COMPONENT color_get_g(COLOR color) {
	return (color & 0x0000ff00) >> 8;
}

static inline COLOR_COMPONENT color_get_b(COLOR color) {
	return (color & 0x000000ff);
}

static inline void color_extract(COLOR color, COLOR_COMPONENT *r, COLOR_COMPONENT *g, COLOR_COMPONENT *b, COLOR_COMPONENT *a) {
	if (a)
		*a = color_get_a(color);
	if (r)
		*r = color_get_r(color);
	if (g)
		*g = color_get_g(color);
	if (b)
		*b = color_get_b(color);
}

static inline COLOR color_create_rgba(COLOR_COMPONENT r, COLOR_COMPONENT g, COLOR_COMPONENT b, COLOR_COMPONENT a) {
	return (a << 24) | (r << 16) | (g << 8) | b;
}

static inline COLOR color_create_rgb(COLOR_COMPONENT r, COLOR_COMPONENT g, COLOR_COMPONENT b) {
	return color_create_rgba(r, g, b, OPAQUE);
}


/* floating-point color components */

typedef float COLORF_COMPONENT;

static const COLORF_COMPONENT TRANSPARENT_F = 0.0f;
static const COLORF_COMPONENT OPAQUE_F = 1.0f;

static inline COLORF_COMPONENT colorf_get_a(COLOR color) {
	return (float)((color & 0xff000000) >> 24) / 255;
}

static inline COLORF_COMPONENT colorf_get_r(COLOR color) {
	return (float)((color & 0x00ff0000) >> 16) / 255;
}

static inline COLORF_COMPONENT colorf_get_g(COLOR color) {
	return (float)((color & 0x0000ff00) >> 8) / 255;
}

static inline COLORF_COMPONENT colorf_get_b(COLOR color) {
	return (float)(color & 0x000000ff) / 255;
}

static inline void colorf_extract(COLOR color, COLORF_COMPONENT *r, COLORF_COMPONENT *g, COLORF_COMPONENT *b, COLORF_COMPONENT *a) {
	if (a)
		*a = colorf_get_a(color);
	if (r)
		*r = colorf_get_r(color);
	if (g)
		*g = colorf_get_g(color);
	if (b)
		*b = colorf_get_b(color);
}

static inline COLOR colorf_create_rgba(COLORF_COMPONENT r, COLORF_COMPONENT g, COLORF_COMPONENT b, COLORF_COMPONENT a) {
	return ((COLOR_COMPONENT)(a * 255) << 24) | 
		   ((COLOR_COMPONENT)(r * 255) << 16) | 
	       ((COLOR_COMPONENT)(g * 255) << 8) | 
	        (COLOR_COMPONENT)(b * 255);
}

static inline COLOR colorf_create_rgb(COLORF_COMPONENT r, COLORF_COMPONENT g, COLORF_COMPONENT b, COLORF_COMPONENT a) {
	return colorf_create_rgba(r, g, b, OPAQUE_F);
}

static inline COLOR color_blend(COLOR source, COLOR destination) {
	int sR = color_get_r(source);
	int sG = color_get_g(source);
	int sB = color_get_b(source);
	int sA = color_get_a(source);
	int dR = color_get_r(destination);
	int dG = color_get_g(destination);
	int dB = color_get_b(destination);
	int dA = color_get_a(destination);

	 // this is equivalent to "glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)"
	return color_create_rgba(
		(COLOR_COMPONENT)(((sR * sA) + (dR * (255 - sA))) / 255),
		(COLOR_COMPONENT)(((sG * sA) + (dG * (255 - sA))) / 255),
		(COLOR_COMPONENT)(((sB * sA) + (dB * (255 - sA))) / 255),
		(COLOR_COMPONENT)(((sA * sA) + (dA * (255 - sA))) / 255)
		);
}

static inline COLOR color_tint(COLOR color, COLOR tint) {
	int cR = color_get_r(color);
	int cG = color_get_g(color);
	int cB = color_get_b(color);
	int cA = color_get_a(color);
	int tR = color_get_r(tint);
	int tG = color_get_g(tint);
	int tB = color_get_b(tint);
	int tA = color_get_a(tint);

	return color_create_rgba(
		(COLOR_COMPONENT)((cR * tR) / 255),
		(COLOR_COMPONENT)((cG * tG) / 255),
		(COLOR_COMPONENT)((cB * tB) / 255),
		(COLOR_COMPONENT)((cA * tA) / 255)
		);
}

static inline COLOR color_tint_alpha(COLOR_COMPONENT alpha, COLOR tint) {
	return color_create_rgba(
		color_get_r(tint),
		color_get_g(tint),
		color_get_b(tint),
		(COLOR_COMPONENT)(((uint32_t)alpha * color_get_a(tint)) / 255)
	);
}

static inline void color_convert_rgba_to_rgb(COLOR rgba, COLOR_COMPONENT *r, COLOR_COMPONENT *g, COLOR_COMPONENT *b) {
	COLOR_COMPONENT a = color_get_a(rgba);
	*r = (COLOR_COMPONENT)(((uint32_t)color_get_r(rgba) * a) / 255);
	*g = (COLOR_COMPONENT)(((uint32_t)color_get_g(rgba) * a) / 255);
	*b = (COLOR_COMPONENT)(((uint32_t)color_get_b(rgba) * a) / 255);
}

/* -- */

#define COLOR_WHITE             (color_create_rgb(255, 255, 255))
#define COLOR_RED               (color_create_rgb(255, 0,   0))
#define COLOR_GREEN             (color_create_rgb(0,   255, 0))
#define COLOR_BLUE              (color_create_rgb(0,   0,   255))
#define COLOR_YELLOW            (color_create_rgb(255, 255, 0))
#define COLOR_CYAN              (color_create_rgb(0,   255, 255))
#define COLOR_MAGENTA           (color_create_rgb(255, 0,   255))
#define COLOR_BLACK             (color_create_rgb(0,   0,   0))

#define COLOR_DOS_BLACK         (color_create_rgb(0,   0,   0))
#define COLOR_DOS_BLUE          (color_create_rgb(0,   0,   170))
#define COLOR_DOS_GREEN         (color_create_rgb(0,   170, 0))
#define COLOR_DOS_CYAN          (color_create_rgb(0,   170, 170))
#define COLOR_DOS_RED           (color_create_rgb(170, 0,   0))
#define COLOR_DOS_MAGENTA       (color_create_rgb(170, 0,   170))
#define COLOR_DOS_BROWN         (color_create_rgb(170, 85,  0))
#define COLOR_DOS_WHITE         (color_create_rgb(170, 170, 170))
#define COLOR_DOS_GRAY          (color_create_rgb(85,  85,  85))
#define COLOR_DOS_LIGHT_BLUE    (color_create_rgb(85,  85,  255))
#define COLOR_DOS_LIGHT_GREEN   (color_create_rgb(85,  255, 64))
#define COLOR_DOS_LIGHT_CYAN    (color_create_rgb(85,  255, 255))
#define COLOR_DOS_LIGHT_RED     (color_create_rgb(255, 85,  85))
#define COLOR_DOS_LIGHT_MAGENTA (color_create_rgb(255, 85,  255))
#define COLOR_DOS_YELLOW        (color_create_rgb(255, 255, 85))
#define COLOR_DOS_BRIGHT_WHITE  (color_create_rgb(255, 255, 255))

#ifdef __cplusplus
}
#endif