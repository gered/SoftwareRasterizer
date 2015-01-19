#include <stdio.h>
#include "vm/vm.h"
#include <fbgfx/surface.h>

int main(int argc, char **argv) {
	log_init();
	input_init();
	WINDOW *window = window_init("test_color_format", 1024, 768, 320, 240);
	if (!window)
		return 1;

	int step = 0;

	while (step < 4) {
		if (!window_do_events(window))
			break;
		if (input_is_key_down(KSYM_ESCAPE))
			break;
		if (input_is_key_pressed(KSYM_SPACE))
			++step;

		// obviously requiring visual confirmation that the color being rendered is
		// the one specified in the code ...
		switch (step) {
			case 0:	surface_clear(window->surface, color_create_rgb(0,   0,   0));   break;
			case 1:	surface_clear(window->surface, color_create_rgb(255, 0,   0));   break;
			case 2:	surface_clear(window->surface, color_create_rgb(0,   255, 0));   break;
			case 3:	surface_clear(window->surface, color_create_rgb(0,   0,   255)); break;
		}

		window_render(window);
	}

	window_destroy(window);
	input_destroy();
	log_end();

	return 0;
}
