#include <stdio.h>
#include <time.h>
#include <fbgfx/surface.h>
#include "vm/vm.h"
#include "fbgfx/surface.h"
#include "fbgfx/clipping.h"
#include "assets/image.h"
#include "vm/vm_window.h"

static inline double get_seconds() {
	return (double)clock() / CLOCKS_PER_SEC;
}

int main(int argc, char** argv) {
	log_init();
	input_init();
	WINDOW *window = window_init("Rasterizer Test", 1280, 960, 320, 240);
	if (!window)
		return 1;

	SURFACE *sprite = image_load_file("/Users/gered/resources/grass01.bmp");
	SURFACE *sprite2 = surface_create(16, 16, SURFACE_FORMAT_ALPHA, SURFACE_FLAGS_NONE);
	surface_clear(sprite2, color_create_rgba(0, 0, 0, 0));
	surface_rect_filled(sprite2, 4, 4, 11, 11, color_create_rgba(0, 0, 0, 128));
	surface_line(sprite2, 0, 0, 15, 15, color_create_rgba(0, 0, 0, 255));

	double before;
	double after;
	int i;

	before = get_seconds();

	for (i = 0; i < 500000; ++i) {
		surface_blit(sprite, window->surface, 0, 0);
		//surface_blit_blend(sprite, window->surface, 0, 0);
	}

	after = get_seconds();

	printf("time taken: %f\n", after - before);

	while (true) {
		if (!window_do_events(window))
			break;
		if (input_is_key_pressed(KSYM_ESCAPE))
			break;

		//surface_clear(window->surface, color_create_rgba(192, 192, 192, 255));
		int x, y;
		for (y = 0; y < window->surface->height; y += 16) {
			for (x = 0; x < window->surface->width; x += 16) {
				surface_blit(sprite, window->surface, x, y);
			}
		}

		surface_line(window->surface, window->viewport_width / 2, window->viewport_height / 2, input_get_mouse_x(), input_get_mouse_y(), COLOR_YELLOW);
		//surface_blit_tint(sprite, window->surface, INPUT_GetMouseX(), INPUT_GetMouseY(), COLOR_DOS_BROWN);
		surface_blit_blend(sprite2, window->surface, 0, 0);
		surface_blit_blend_tint(sprite2, window->surface, 16, 0, color_create_rgba(255, 0, 0, 64));

		window_render(window);
	}
	
	window_destroy(window);
	input_destroy();
	log_end();
	
	return 0;
}
