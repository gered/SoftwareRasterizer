#include "fbgfx/surface.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

SURFACE *top_screen_surface = NULL;
SURFACE *bottom_screen_surface = NULL;

void surface_init() {
	if (top_screen_surface || bottom_screen_surface)
		return;   // TODO: assert

	/*
	top_screen_surface = surface_wrap_existing(
		gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL), 
		400, 240, 
		SURFACE_FORMAT_RGB,
		SURFACE_FLAGS_SIDEWAYS_BUFFER
		);

	bottom_screen_surface = surface_wrap_existing(
		gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL), 
		320, 240, 
		SURFACE_FORMAT_RGB,
		SURFACE_FLAGS_SIDEWAYS_BUFFER
		);
	*/
}

void surface_exit() {
	if (top_screen_surface)
		surface_destroy(top_screen_surface);
	if (bottom_screen_surface)
		surface_destroy(bottom_screen_surface);

	top_screen_surface = NULL;
	bottom_screen_surface = NULL;
}

void surface_swap_framebuffers() {
	/*
	top_screen_surface->pixels = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
	bottom_screen_surface->pixels = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
	*/
}

static uint32_t get_index(const SURFACE *surface, int x, int y) {
	if (surface->flags & SURFACE_FLAGS_SIDEWAYS_BUFFER)
		return (uint32_t)((x * surface->height) + (surface->height - y - 1)) * surface->bytes_per_pixel;
	else
		return (uint32_t)(x + (y * surface->width)) * surface->bytes_per_pixel;
}

void setup_initial_surface_properties(SURFACE *surface, int width, int height, SURFACE_FORMAT format, SURFACE_FLAGS flags) {
	surface->width = width;
	surface->height = height;
	surface->format = format;
	surface->flags = flags;

	surface->bytes_per_pixel = surface->format / 8;

	surface->clip_region = rect_create(0, 0, surface->width, surface->height);

	// lazy calculations for the win
	uint32_t reference_index = get_index(surface, 0, 0);
	surface->top_left_index = reference_index;
	surface->x_inc = get_index(surface, 1, 0) - reference_index;
	surface->y_inc = get_index(surface, 0, 1) - reference_index;
}

SURFACE* surface_create(int width, int height, SURFACE_FORMAT format, SURFACE_FLAGS flags) {
	SURFACE *surface = malloc(sizeof(SURFACE));
	if (surface == NULL)
		return NULL;

	setup_initial_surface_properties(surface, width, height, format, flags);

	size_t pixels_buffer_size = surface_get_buffer_size(surface);

	surface->pixels = malloc(pixels_buffer_size);
	if (surface->pixels == NULL) {
		free(surface);
		return NULL;
	}

	surface->wrapped_buffer = false;
	
	memset(surface->pixels, 0, pixels_buffer_size);
	
	return surface;
}

SURFACE* surface_wrap_existing(void *framebuffer, int width, int height, SURFACE_FORMAT format, SURFACE_FLAGS flags) {
	SURFACE *surface = malloc(sizeof(SURFACE));
	if (surface == NULL)
		return NULL;

	setup_initial_surface_properties(surface, width, height, format, flags);

	surface->pixels = (COLOR_COMPONENT*)framebuffer;
	surface->wrapped_buffer = true;

	// note: not clearing framebuffer. since we're using existing memory, maybe the
	// calling code wants it the way it is now? don't want to assume anything!

	return surface;
}

void surface_resize(SURFACE *surface, int width, int height) {
	if (!surface)
		return;  // TODO: assert
	if (surface->wrapped_buffer)
		return;  // TODO: assert

	setup_initial_surface_properties(surface, width, height, surface->format, surface->flags);
	size_t new_pixels_buffer_size = surface_get_buffer_size(surface);

	COLOR_COMPONENT *pixels = malloc(new_pixels_buffer_size);
	if (pixels == NULL)
		return;  // TODO: assert

	free(surface->pixels);
	surface->pixels = pixels;

	memset(surface->pixels, 0, new_pixels_buffer_size);
}

void surface_destroy(SURFACE *surface) {
	if (!surface)
		return;  // TODO: assert

	if (!surface->wrapped_buffer)
		free(surface->pixels);
	free(surface);
}

//////////////

void surface_clear(SURFACE *surface, COLOR color) {
	size_t pixels_buffer_size = surface_get_buffer_size(surface);
	COLOR_COMPONENT *p = surface->pixels;
	COLOR_COMPONENT *end = p + pixels_buffer_size;

	COLOR_COMPONENT r, g, b, a;
	color_extract(color, &r, &g, &b, &a);

	switch (surface->format) {
		case SURFACE_FORMAT_ALPHA:
			memset(surface->pixels, a, pixels_buffer_size);
			break;

		case SURFACE_FORMAT_RGB:
			for (; p < end; p += 3) {
				p[0] = b;
				p[1] = g;
				p[2] = r;
			}
			break;

		case SURFACE_FORMAT_RGBA:
			for (; p < end; p += 4) {
				p[0] = b;
				p[1] = g;
				p[2] = r;
				p[3] = a;
			}
			break;
	}
}
