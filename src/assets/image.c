#include "assets/image.h"
#include "../vendor/stb_image.h"
#include "fbgfx/surface.h"
#include "types.h"
#include <stdio.h>

SURFACE* create_surface_from_pixels(uint8_t *pixels, int width, int height, int bpp) {
	SURFACE *surface = NULL;
	SURFACE_FORMAT format;

	switch (bpp) {
		case 1: format = SURFACE_FORMAT_ALPHA; break;
		case 3: format = SURFACE_FORMAT_RGB; break;
		case 4: format = SURFACE_FORMAT_RGBA; break;

		default:
			stbi_image_free(pixels);
			return NULL;
	}

	surface = surface_wrap_existing(pixels, width, height, format, 0);
	if (!surface) {
		stbi_image_free(pixels);
		return NULL;
	}

	// HACK: this one little time, we're going go manually toggle this setting in a way we shouldn't be!
	//       (all that stbi_image_free() does is call free(), so it's fine if we leave cleanup of the
	//       stb_image pixel buffer to surface_destroy() ...)
	surface->wrapped_buffer = false;

	surface_convert_rgb_to_bgr(surface);
	return surface;
}

SURFACE* image_load_file(const char *filename) {
	if (!filename)
		return NULL;

	int width;
	int height;
	int bpp;
	uint8_t *pixels = stbi_load(filename, &width, &height, &bpp, 0);
	if (!pixels)
		return NULL;
	else
		return create_surface_from_pixels(pixels, width, height, bpp);
}

SURFACE* image_load_memory(const void *image_file_bin, uint32_t size) {
	if (!image_file_bin || !size)
		return NULL;

	int width;
	int height;
	int bpp;
	uint8_t *pixels = stbi_load_from_memory(image_file_bin, size, &width, &height, &bpp, 0);
	if (!pixels)
		return NULL;
	else
		return create_surface_from_pixels(pixels, width, height, bpp);
}

