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

int main(int argc, char **argv) {
	test_rgba(SURFACE_FLAGS_NONE);
	test_rgb(SURFACE_FLAGS_NONE);
	test_alpha(SURFACE_FLAGS_NONE);

	return 0;
}
