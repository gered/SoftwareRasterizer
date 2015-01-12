#include <stdio.h>
#include "vm/vm.h"
#include <fbgfx/surface.h>

int main(int argc, char **argv) {
	log_init();
	input_init();
	WINDOW *window = window_init("Rasterizer Test", 1280, 960, 320, 240);
	if (!window)
		return 1;

	while (true) {
		if (!window_do_events(window))
			break;
		if (input_is_key_down(KSYM_ESCAPE))
			break;

		window_render(window);
	}

	window_destroy(window);
	input_destroy();
	log_end();

	return 0;
}
