#include "fbgfx/surface.h"
#include "fbgfx/clipping.h"
#include "math/rect.h"
#include "types.h"
#include <stdlib.h>

#define SIGN(x) ((x < 0) ? -1 : ((x > 0) ? 1 : 0))


/**
 * PIXEL PLOTTING
 */

static inline void set_pixel_fast(SURFACE *surface, int x, int y, COLOR color) {
	switch (surface->format) {
		case SURFACE_FORMAT_ALPHA: surface_set_pixel_alpha_fast(surface, x, y, color); break;
		case SURFACE_FORMAT_RGB:   surface_set_pixel_rgb_fast(surface, x, y, color); break;
		case SURFACE_FORMAT_RGBA:  surface_set_pixel_rgba_fast(surface, x, y, color); break;
	}
}

static inline COLOR get_pixel_fast(const SURFACE *surface, int x, int y) {
	switch (surface->format) {
		case SURFACE_FORMAT_ALPHA: return surface_get_pixel_alpha_fast(surface, x, y);
		case SURFACE_FORMAT_RGB:   return surface_get_pixel_rgb_fast(surface, x, y);
		case SURFACE_FORMAT_RGBA:  return surface_get_pixel_rgba_fast(surface, x, y);
	}
}

void surface_set_pixel(SURFACE *surface, int x, int y, COLOR color) {
	if (!point_in_bounds(&surface->clip_region, x, y))
		return;

	set_pixel_fast(surface, x, y, color);
}

void surface_set_pixel_fast(SURFACE *surface, int x, int y, COLOR color) {
	set_pixel_fast(surface, x, y, color);
}

COLOR surface_get_pixel(const SURFACE *surface, int x, int y) {
	if (!point_in_bounds(&surface->clip_region, x, y))
		return 0;

	return get_pixel_fast(surface, x, y);
}

COLOR surface_get_pixel_fast(SURFACE *surface, int x, int y) {
	return get_pixel_fast(surface, x, y);
}



/**
 * LINE DRAWING
 */

#define DRAW_LINE(surface, x1, y1, x2, y2, color, draw_pixel) \
	do {                                                      \
		int delta_x = x2 - x1;                                \
		int delta_y = y2 - y1;                                \
		int delta_x_abs = abs(delta_x);                       \
		int delta_y_abs = abs(delta_y);                       \
		int delta_x_sign = SIGN(delta_x);                     \
		int delta_y_sign = SIGN(delta_y);                     \
		int x = delta_y_abs / 2;                              \
		int y = delta_x_abs / 2;                              \
		int px = x1;                                          \
		int py = y1;                                          \
		                                                      \
		draw_pixel(surface, px, py, color);                   \
		                                                      \
		if (delta_x_abs >= delta_y_abs) {                     \
			int i;                                            \
			for(i = 0; i < delta_x_abs; ++i) {                \
				y += delta_y_abs;                             \
				                                              \
				if (y >= delta_x_abs) {                       \
					y -= delta_x_abs;                         \
					py += delta_y_sign;                       \
				}                                             \
				                                              \
				px += delta_x_sign;                           \
				draw_pixel(surface, px, py, color);           \
			}                                                 \
			                                                  \
		} else {                                              \
			int i;                                            \
			for(i = 0; i < delta_y_abs; ++i) {                \
				x += delta_x_abs;                             \
				                                              \
				if (x >= delta_y_abs) {                       \
					x -= delta_y_abs;                         \
					px += delta_x_sign;                       \
				}                                             \
				                                              \
				py += delta_y_sign;                           \
				draw_pixel(surface, px, py, color);           \
			}                                                 \
		}                                                     \
	} while (0)

void surface_line_fast(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color) {
	switch (surface->format) {
		case SURFACE_FORMAT_ALPHA: DRAW_LINE(surface, x1, y1, x2, y2, color, surface_set_pixel_alpha_fast); break;
		case SURFACE_FORMAT_RGB:   DRAW_LINE(surface, x1, y1, x2, y2, color, surface_set_pixel_rgb_fast); break;
		case SURFACE_FORMAT_RGBA:  DRAW_LINE(surface, x1, y1, x2, y2, color, surface_set_pixel_rgba_fast); break;
	}
}

void surface_line(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color) {
	if (!line_in_bounds(&surface->clip_region, x1, y1, x2, y2))
		return;

	DRAW_LINE(surface, x1, y1, x2, y2, color, surface_set_pixel);
}

#define DRAW_FAST_HLINE(surface, x1, x2, y, color, fmt)    \
	do {                                                   \
		if (x2 < x1)                                       \
			SWAP(int, x1, x2);                             \
		                                                   \
		int x;                                             \
		uint32_t i = surface_get_index_of(surface, x1, y); \
		for (x = x1; x <= x2; ++x, i += surface->x_inc)    \
			SET_PIXEL_IDX_FAST(fmt, surface, i, color);    \
	} while(0)

void surface_hline_fast(SURFACE *surface, int x1, int x2, int y, COLOR color) {
	switch (surface->format) {
		case SURFACE_FORMAT_ALPHA: DRAW_FAST_HLINE(surface, x1, x2, y, color, alpha); break;
		case SURFACE_FORMAT_RGB:   DRAW_FAST_HLINE(surface, x1, x2, y, color, rgb); break;
		case SURFACE_FORMAT_RGBA:  DRAW_FAST_HLINE(surface, x1, x2, y, color, rgba); break;
	}
}

void surface_hline(SURFACE *surface, int x1, int x2, int y, COLOR color) {
	if (!clamp_to_region(&surface->clip_region, &x1, &y, &x2, &y))
		return;

	surface_hline_fast(surface, x1, x2, y, color);
}

#define DRAW_FAST_VLINE(surface, x, y1, y2, color, fmt)    \
	do {                                                   \
		if (y2 < y1)                                       \
			SWAP(int, y1, y2);                             \
		                                                   \
		int y;                                             \
		uint32_t i = surface_get_index_of(surface, x, y1); \
		for (y = y1; y <= y2; ++y, i += surface->y_inc)    \
			SET_PIXEL_IDX_FAST(fmt, surface, i, color);    \
	} while (0)

void surface_vline_fast(SURFACE *surface, int x, int y1, int y2, COLOR color) {
	switch (surface->format) {
		case SURFACE_FORMAT_ALPHA: DRAW_FAST_VLINE(surface, x, y1, y2, color, alpha); break;
		case SURFACE_FORMAT_RGB:   DRAW_FAST_VLINE(surface, x, y1, y2, color, rgb); break;
		case SURFACE_FORMAT_RGBA:  DRAW_FAST_VLINE(surface, x, y1, y2, color, rgba); break;
	}
}

void surface_vline(SURFACE *surface, int x, int y1, int y2, COLOR color) {
	if (!clamp_to_region(&surface->clip_region, &x, &y1, &x, &y2))
		return;

	surface_vline_fast(surface, x, y1, y2, color);
}



/**
 * RECTANGLE DRAWING
 */

void surface_rect(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color) {
	if (!line_in_bounds(&surface->clip_region, x1, y1, x2, y2))
		return;

	if (x2 < x1)
		SWAP(int, x1, x2);
	if (y2 < y1)
		SWAP(int, y1, y2);

	int x;
	for (x = x1; x <= x2; ++x) {
		surface_set_pixel(surface, x, y1, color);
		surface_set_pixel(surface, x, y2, color);
	}

	int y;
	for (y = y1; y <= y2; ++y) {
		surface_set_pixel(surface, x1, y, color);
		surface_set_pixel(surface, x2, y, color);
	}
}

#define DRAW_FAST_RECT(surface, x1, y1, x2, y2, color, fmt) \
	do {                                                    \
		if (x2 < x1)                                        \
			SWAP(int, x1, x2);                              \
		if (y2 < y1)                                        \
			SWAP(int, y1, y2);                              \
		                                                    \
		uint32_t a;                                         \
		uint32_t b;                                         \
		                                                    \
		int x = x1;                                         \
		a = surface_get_index_of(surface, x, y1);           \
		b = surface_get_index_of(surface, x, y2);           \
		for (; x <= x2; ++x) {                              \
			SET_PIXEL_IDX_FAST(fmt, surface, a, color);     \
			SET_PIXEL_IDX_FAST(fmt, surface, b, color);     \
			a += surface->x_inc;                            \
			b += surface->x_inc;                            \
		}                                                   \
		                                                    \
		int y = y1;                                         \
		a = surface_get_index_of(surface, x1, y);           \
		b = surface_get_index_of(surface, x2, y);           \
		for (; y <= y2; ++y) {                              \
			SET_PIXEL_IDX_FAST(fmt, surface, a, color);     \
			SET_PIXEL_IDX_FAST(fmt, surface, b, color);     \
			a += surface->y_inc;                            \
			b += surface->y_inc;                            \
		}                                                   \
	} while (0)

void surface_rect_fast(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color) {
	switch (surface->format) {
		case SURFACE_FORMAT_ALPHA: DRAW_FAST_RECT(surface, x1, y1, x2, y2, color, alpha); break;
		case SURFACE_FORMAT_RGB:   DRAW_FAST_RECT(surface, x1, y1, x2, y2, color, rgb); break;
		case SURFACE_FORMAT_RGBA:  DRAW_FAST_RECT(surface, x1, y1, x2, y2, color, rgba); break;
	}
}



/**
 * FILLED RECTANGLE DRAWING
 */

#define DRAW_FAST_RECT_FILLED(surface, x1, y1, x2, y2, color, fmt) \
	do {                                                           \
		if (x2 < x1)                                               \
			SWAP(int, x1, x2);                                     \
		if (y2 < y1)                                               \
			SWAP(int, y1, y2);                                     \
		                                                           \
		uint32_t x_index;                                          \
		uint32_t y_index;                                          \
		int x;                                                     \
		int y;                                                     \
		                                                           \
		y_index = surface_get_index_of(surface, x1, y1);           \
		for (y = y1; y <= y2; ++y) {                               \
			x_index = y_index;                                     \
			for (x = x1; x <= x2; ++x) {                           \
				SET_PIXEL_IDX_FAST(fmt, surface, x_index, color);  \
				x_index += surface->x_inc;                         \
			}                                                      \
			                                                       \
			y_index += surface->y_inc;                             \
		}                                                          \
	} while (0)

void surface_rect_filled_fast(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color) {
	switch (surface->format) {
		case SURFACE_FORMAT_ALPHA: DRAW_FAST_RECT_FILLED(surface, x1, y1, x2, y2, color, alpha); break;
		case SURFACE_FORMAT_RGB:   DRAW_FAST_RECT_FILLED(surface, x1, y1, x2, y2, color, rgb); break;
		case SURFACE_FORMAT_RGBA:  DRAW_FAST_RECT_FILLED(surface, x1, y1, x2, y2, color, rgba); break;
	}
}

void surface_rect_filled(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color) {
	if (!clamp_to_region(&surface->clip_region, &x1, &y1, &x2, &y2))
		return;

	surface_rect_filled_fast(surface, x1, y1, x2, y2, color);
}
