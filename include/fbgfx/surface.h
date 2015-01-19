#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../types.h"
#include "../math/rect.h"
#include "color.h"

typedef enum {
	SURFACE_FORMAT_ALPHA = 8,
	SURFACE_FORMAT_RGB   = 24,
	SURFACE_FORMAT_RGBA  = 32
} SURFACE_FORMAT;

typedef enum {
	SURFACE_FLAGS_NONE = 0,
	SURFACE_FLAGS_SIDEWAYS_BUFFER = 1
} SURFACE_FLAGS;

typedef struct SURFACE {
	SURFACE_FLAGS flags;
	COLOR_COMPONENT *pixels;
	bool wrapped_buffer;
	SURFACE_FORMAT format;
	int bytes_per_pixel;
	int width;
	int height;
	uint32_t top_left_index;
	int x_inc;
	int y_inc;
	RECT clip_region;
} SURFACE;

extern SURFACE *top_screen_surface;
extern SURFACE *bottom_screen_surface;

void surface_init();
void surface_exit();

void surface_swap_framebuffers();

SURFACE* surface_create(int width, int height, SURFACE_FORMAT format, SURFACE_FLAGS flags);
SURFACE* surface_wrap_existing(void *framebuffer, int width, int height, SURFACE_FORMAT format, SURFACE_FLAGS flags);
void surface_resize(SURFACE *surface, int width, int height);
void surface_destroy(SURFACE *surface);

void surface_convert_rgb_to_bgr(SURFACE *surface);

void surface_clear(SURFACE *surface, COLOR color);

void surface_set_pixel(SURFACE *surface, int x, int y, COLOR color);
void surface_set_pixel_fast(SURFACE *surface, int x, int y, COLOR color);
COLOR surface_get_pixel(const SURFACE *surface, int x, int y);
COLOR surface_get_pixel_fast(SURFACE *surface, int x, int y);

void surface_line(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color);
void surface_line_fast(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color);
void surface_hline(SURFACE *surface, int x1, int x2, int y, COLOR color);
void surface_hline_fast(SURFACE *surface, int x1, int x2, int y, COLOR color);
void surface_vline(SURFACE *surface, int x, int y1, int y2, COLOR color);
void surface_vline_fast(SURFACE *surface, int x, int y1, int y2, COLOR color);
void surface_rect(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color);
void surface_rect_fast(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color);
void surface_rect_filled(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color);
void surface_rect_filled_fast(SURFACE *surface, int x1, int y1, int x2, int y2, COLOR color);

void surface_blit_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y);
void surface_blit_blend_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y);
void surface_blit_trans_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y);
void surface_blit_tint_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y, COLOR tint);
void surface_blit_blend_tint_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y, COLOR tint);

#define GET_PIXEL_FAST(fmt, ...) surface_get_pixel_ ## fmt ## _fast(__VA_ARGS__)
#define SET_PIXEL_FAST(fmt, ...) surface_set_pixel_ ## fmt ## _fast(__VA_ARGS__)
#define GET_PIXEL_IDX_FAST(fmt, ...) surface_get_pixel_idx_ ## fmt ## _fast(__VA_ARGS__)
#define SET_PIXEL_IDX_FAST(fmt, ...) surface_set_pixel_idx_ ## fmt ## _fast(__VA_ARGS__)

////////////////

static inline uint32_t surface_get_buffer_size(const SURFACE *surface) {
	return (uint32_t)(surface->width * surface->height) * surface->bytes_per_pixel;
}

static inline uint32_t surface_get_index_of(const SURFACE *surface, int x, int y) {
	return (uint32_t)(surface->top_left_index + (surface->x_inc * x) + (surface->y_inc * y));
}

static inline COLOR_COMPONENT* surface_get_pointer_to(const SURFACE *surface, int x, int y) {
	return surface->pixels + surface_get_index_of(surface, x, y);
}

////////////////

static inline void surface_set_pixel_idx_alpha_fast(SURFACE *surface, uint32_t index, COLOR color) {
	COLOR_COMPONENT alpha = color_get_a(color);
	COLOR_COMPONENT *p = (surface->pixels + index);
	*p = alpha;
}

static inline void surface_set_pixel_idx_rgb_fast(SURFACE *surface, uint32_t index, COLOR color) {
	COLOR_COMPONENT *p = (surface->pixels + index);
	color_convert_rgba_to_rgb(color, (p + 2), (p + 1), p);
}

static inline void surface_set_pixel_idx_rgba_fast(SURFACE *surface, uint32_t index, COLOR color) {
	COLOR_COMPONENT *p = (surface->pixels + index);
	color_extract(color, (p + 2), (p + 1), p, (p + 3));
}

static inline COLOR surface_get_pixel_idx_alpha_fast(const SURFACE *surface, uint32_t index) {
	COLOR_COMPONENT *p = (surface->pixels + index);
	return color_create_rgba(0, 0, 0, *p);
}

static inline COLOR surface_get_pixel_idx_rgb_fast(const SURFACE *surface, uint32_t index) {
	COLOR_COMPONENT *p = (surface->pixels + index);
	COLOR_COMPONENT r = p[2];
	COLOR_COMPONENT g = p[1];
	COLOR_COMPONENT b = p[0];
	return color_create_rgb(r, g, b);
}

static inline COLOR surface_get_pixel_idx_rgba_fast(const SURFACE *surface, uint32_t index) {
	COLOR_COMPONENT *p = (surface->pixels + index);
	COLOR_COMPONENT a = p[3];
	COLOR_COMPONENT r = p[2];
	COLOR_COMPONENT g = p[1];
	COLOR_COMPONENT b = p[0];
	return color_create_rgba(r, g, b, a);
}

static inline void surface_set_pixel_alpha_fast(SURFACE *surface, int x, int y, COLOR color) {
	uint32_t index = surface_get_index_of(surface, x, y);
	surface_set_pixel_idx_alpha_fast(surface, index, color);
}

static inline void surface_set_pixel_rgb_fast(SURFACE *surface, int x, int y, COLOR color) {
	uint32_t index = surface_get_index_of(surface, x, y);
	surface_set_pixel_idx_rgb_fast(surface, index, color);
}

static inline void surface_set_pixel_rgba_fast(SURFACE *surface, int x, int y, COLOR color) {
	uint32_t index = surface_get_index_of(surface, x, y);
	surface_set_pixel_idx_rgba_fast(surface, index, color);
}

static inline COLOR surface_get_pixel_alpha_fast(const SURFACE *surface, int x, int y) {
	uint32_t index = surface_get_index_of(surface, x, y);
	return surface_get_pixel_idx_alpha_fast(surface, index);
}

static inline COLOR surface_get_pixel_rgb_fast(const SURFACE *surface, int x, int y) {
	uint32_t index = surface_get_index_of(surface, x, y);
	return surface_get_pixel_idx_rgb_fast(surface, index);
}

static inline COLOR surface_get_pixel_rgba_fast(const SURFACE *surface, int x, int y) {
	uint32_t index = surface_get_index_of(surface, x, y);
	return surface_get_pixel_idx_rgba_fast(surface, index);
}

static inline void surface_blit(SURFACE *source, SURFACE *destination, int x, int y) {
	surface_blit_region(source, destination, 0, 0, source->width, source->height, x, y);
}

static inline void surface_blit_blend(SURFACE *source, SURFACE *destination, int x, int y) {
	surface_blit_blend_region(source, destination, 0, 0, source->width, source->height, x, y);
}

static inline void surface_blit_trans(SURFACE *source, SURFACE *destination, int x, int y) {
	surface_blit_trans_region(source, destination, 0, 0, source->width, source->height, x, y);
}

static inline void surface_blit_tint(SURFACE *source, SURFACE *destination, int x, int y, COLOR tint) {
	surface_blit_tint_region(source, destination, 0, 0, source->width, source->height, x, y, tint);
}

static inline void surface_blit_blend_tint(SURFACE *source, SURFACE *destination, int x, int y, COLOR tint) {
	surface_blit_blend_tint_region(source, destination, 0, 0, source->width, source->height, x, y, tint);
}

#ifdef __cplusplus
}
#endif
