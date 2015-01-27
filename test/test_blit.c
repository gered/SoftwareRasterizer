#include <stdio.h>
#include "vm/vm.h"
#include <fbgfx/surface.h>
#include <vm/vm_window.h>

int main(int argc, char **argv) {
	log_init();
	input_init();
	WINDOW *window = window_init("test_blit", 1024, 768, 640, 480);
	if (!window)
		return 1;

	// test surface we will render with the different blit functions
	SURFACE *bmp = surface_create(64, 64, SURFACE_FORMAT_RGBA, SURFACE_FLAGS_NONE);
	surface_clear(bmp, color_create_rgba(0, 0, 0, 0));
	surface_rect_filled(bmp, 16, 0, 47, 31, color_create_rgba(255, 96, 96, 255));
	surface_rect_filled(bmp, 0, 32, 31, 63, color_create_rgba(96, 255, 96, 192));
	surface_rect_filled(bmp, 32, 32, 63, 63, color_create_rgba(96, 96, 255, 64));


	surface_clear(window->surface, COLOR_DOS_GRAY);

	surface_blit(bmp, window->surface, 10, 10);
	surface_blit_blend(bmp, window->surface, 84, 10);
	surface_blit_trans(bmp, window->surface, 158, 10);
	surface_blit_tint(bmp, window->surface, 232, 10, COLOR_RED);
	surface_blit_blend_tint(bmp, window->surface, 306, 10, COLOR_RED);

	surface_blit_region(bmp, window->surface, 8, 8, 48, 48, 10, 84);
	surface_blit_blend_region(bmp, window->surface, 8, 8, 48, 48, 84, 84);
	surface_blit_trans_region(bmp, window->surface, 8, 8, 48, 48, 158, 84);
	surface_blit_tint_region(bmp, window->surface, 8, 8, 48, 48, 232, 84, COLOR_RED);
	surface_blit_blend_tint_region(bmp, window->surface, 8, 8, 48, 48, 306, 84, COLOR_RED);
	

	while (true) {
		if (!window_do_events(window))
			break;
		if (input_is_key_down(KSYM_ESCAPE))
			break;

		window_render(window);
	}

	surface_destroy(bmp);

	window_destroy(window);
	input_destroy();
	log_end();

	return 0;
}
