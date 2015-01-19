#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fbgfx/surface.h>

void test_rgba(SURFACE_FLAGS flags) {
	SURFACE *surface = surface_create(100, 100, SURFACE_FORMAT_RGBA, flags);
	assert(surface != NULL);

	// set + get
	{
		COLOR color = color_create_rgba(255, 0, 0, 255);
		surface_set_pixel(surface, 20, 10, color);
		COLOR read_color = surface_get_pixel(surface, 20, 10);
		assert(read_color != 0);
		assert(color == read_color);
	}

	// get (out of bounds, clipped)
	{
		COLOR read_color = surface_get_pixel(surface, -1, -1);
		assert(read_color == 0);
	}

	surface_destroy(surface);
}

void test_rgb(SURFACE_FLAGS flags) {
	SURFACE *surface = surface_create(100, 100, SURFACE_FORMAT_RGB, flags);
	assert(surface != NULL);

	// set + get
	{
		COLOR color = color_create_rgb(255, 0, 0);
		surface_set_pixel(surface, 20, 10, color);
		COLOR read_color = surface_get_pixel(surface, 20, 10);
		assert(read_color != 0);
		assert(color == read_color);
	}

	// get (out of bounds, clipped)
	{
		COLOR read_color = surface_get_pixel(surface, -1, -1);
		assert(read_color == 0);
	}

	surface_destroy(surface);
}

void test_alpha(SURFACE_FLAGS flags) {
	SURFACE *surface = surface_create(100, 100, SURFACE_FORMAT_ALPHA, flags);
	assert(surface != NULL);

	// set + get
	// destination surface is alpha-only. setting RGB/RGBA colors drops the RGB portion
	// and keeps only the alpha component, setting it in the destination
	{
		COLOR color = color_create_rgba(255, 0, 0, 255);
		surface_set_pixel(surface, 20, 10, color);
		COLOR read_color = surface_get_pixel(surface, 20, 10);
		assert(read_color != 0);
		assert(color != read_color);
		assert(read_color = color_create_rgba(0, 0, 0, 255));
	}

	// get (out of bounds, clipped)
	{
		COLOR read_color = surface_get_pixel(surface, -1, -1);
		assert(read_color == 0);
	}

	surface_destroy(surface);
}

void test_coords_and_offsets(SURFACE_FLAGS flags) {
	SURFACE *surface = surface_create(100, 100, SURFACE_FORMAT_RGBA, flags);
	assert(surface != NULL);

	COLOR color = color_create_rgba(0, 255, 0, 255);
	int x = 3;
	int y = 42;
	// NOTE: due to the way that surfaces created with SURFACE_FLAGS_SIDEWAYS_BUFFER
	//       after indexed, it is important to start off with a calculated index via
	//       this function. then you can modify the index via x_inc and y_inc
	//       (x = 0, y = 0 is _not_ equal to index = 0 in a sideways buffer)
	uint32_t index = surface_get_index_of(surface, x, y);

	// testing first sets a pixel using x,y coordinates, then reads
	// it back using an offset set to be equivalent to the x,y coords
	// using the surface's x_inc and y_inc properties.

	assert(color != surface_get_pixel_idx_rgba_fast(surface, index));
	surface_set_pixel(surface, x, y, color);
	assert(color == surface_get_pixel_idx_rgba_fast(surface, index));

	x += 10;
	index += (surface->x_inc * 10);

	assert(color != surface_get_pixel_idx_rgba_fast(surface, index));
	surface_set_pixel(surface, x, y, color);
	assert(color == surface_get_pixel_idx_rgb_fast(surface, index));

	y += 15;
	index += (surface->y_inc * 15);

	assert(color != surface_get_pixel_idx_rgba_fast(surface, index));
	surface_set_pixel(surface, x, y, color);
	assert(color == surface_get_pixel_idx_rgb_fast(surface, index));

	x -= 7;
	index -= (surface->x_inc * 7);

	assert(color != surface_get_pixel_idx_rgba_fast(surface, index));
	surface_set_pixel(surface, x, y, color);
	assert(color == surface_get_pixel_idx_rgb_fast(surface, index));

	y -= 9;
	index -= (surface->y_inc * 9);

	assert(color != surface_get_pixel_idx_rgba_fast(surface, index));
	surface_set_pixel(surface, x, y, color);
	assert(color == surface_get_pixel_idx_rgb_fast(surface, index));

	surface_destroy(surface);
}

void test_increments(SURFACE_FLAGS flags) {
	SURFACE *surface = surface_create(1024, 1024, SURFACE_FORMAT_RGBA, flags);
	assert(surface != NULL);

	int x = 0;
	int y = 0;
	uint32_t index = surface_get_index_of(surface, x, y);

	// this is a kind of silly test, but meh, a nice simple-ish validation
	// over a wide range of possible coordinates

	for (; y < surface->height; ++y) {
		uint32_t temp = index;

		for (; x < surface->width; ++x) {
			uint32_t this_index = surface_get_index_of(surface, x, y);
			assert(index == this_index);
			index += surface->x_inc;
		}

		// because the previous x_inc increments make this unnecessary, but we want to test
		// this increment too ...
		index = temp;
		index += surface->y_inc;
	}

	surface_destroy(surface);
}

int main(int argc, char **argv) {
	test_increments(SURFACE_FLAGS_NONE);
	test_increments(SURFACE_FLAGS_SIDEWAYS_BUFFER);
	test_coords_and_offsets(SURFACE_FLAGS_NONE);
	test_coords_and_offsets(SURFACE_FLAGS_SIDEWAYS_BUFFER);
	test_rgba(SURFACE_FLAGS_NONE);
	test_rgba(SURFACE_FLAGS_SIDEWAYS_BUFFER);
	test_rgb(SURFACE_FLAGS_NONE);
	test_rgb(SURFACE_FLAGS_SIDEWAYS_BUFFER);
	test_alpha(SURFACE_FLAGS_NONE);
	test_alpha(SURFACE_FLAGS_SIDEWAYS_BUFFER);

	return 0;
}
