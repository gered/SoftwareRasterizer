#include "fbgfx/surface.h"
#include "types.h"

void surface_convert_rgb_to_bgr(SURFACE *surface) {
	if (surface->format != SURFACE_FORMAT_RGB && surface->format != SURFACE_FORMAT_RGBA)
		return;

	uint32_t buffer_size = surface_get_buffer_size(surface);

	COLOR_COMPONENT *p = surface->pixels;
	COLOR_COMPONENT *end = p + buffer_size;
	for (; p < end; p += surface->bytes_per_pixel) {
		SWAP(COLOR_COMPONENT, p[0], p[2]);
	}
}

