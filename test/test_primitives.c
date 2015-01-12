#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vm/vm.h"
#include <fbgfx/surface.h>

#define RND(low, high) (rand() % ((high - low) + 1) + low)

#define RND_COLOR (color_create_rgb(RND(0, 255), RND(0, 255), RND(0, 255)))

#define RND_TEST_X(surface) (RND(surface->clip_region.x, surface->clip_region.x + surface->clip_region.width - 1))
#define RND_TEST_Y(surface) (RND(surface->clip_region.y, surface->clip_region.y + surface->clip_region.height - 1))
#define RND_CLIP_TEST_X(surface) (RND(surface->clip_region.x, surface->clip_region.x + (surface->clip_region.width * 2)) - surface->clip_region.width / 2)
#define RND_CLIP_TEST_Y(surface) (RND(surface->clip_region.y, surface->clip_region.y + (surface->clip_region.height * 2)) - surface->clip_region.height / 2)

void test_pixels(SURFACE *surface) {
	int x = RND_CLIP_TEST_X(surface);
	int y = RND_CLIP_TEST_Y(surface);

	surface_set_pixel(surface, x, y, RND_COLOR);
}

void test_pixels_fast(SURFACE *surface) {
	int x = RND_TEST_X(surface);
	int y = RND_TEST_Y(surface);

	surface_set_pixel_fast(surface, x, y, RND_COLOR);
}

void test_lines(SURFACE *surface) {
	int x1 = RND_CLIP_TEST_X(surface);
	int x2 = RND_CLIP_TEST_X(surface);
	int y1 = RND_CLIP_TEST_Y(surface);
	int y2 = RND_CLIP_TEST_Y(surface);

	surface_line(surface, x1, y1, x2, y2, RND_COLOR);
}

void test_lines_fast(SURFACE *surface) {
	int x1 = RND_TEST_X(surface);
	int x2 = RND_TEST_X(surface);
	int y1 = RND_TEST_Y(surface);
	int y2 = RND_TEST_Y(surface);

	surface_line_fast(surface, x1, y1, x2, y2, RND_COLOR);
}

void test_h_lines(SURFACE *surface) {
	int x1 = RND_CLIP_TEST_X(surface);
	int x2 = RND_CLIP_TEST_X(surface);
	int y = RND_CLIP_TEST_Y(surface);

	surface_hline(surface, x1, x2, y, RND_COLOR);
}

void test_h_lines_fast(SURFACE *surface) {
	int x1 = RND_TEST_X(surface);
	int x2 = RND_TEST_X(surface);
	int y = RND_TEST_Y(surface);

	surface_hline_fast(surface, x1, x2, y, RND_COLOR);
}

void test_v_lines(SURFACE *surface) {
	int y1 = RND_CLIP_TEST_Y(surface);
	int y2 = RND_CLIP_TEST_Y(surface);
	int x = RND_CLIP_TEST_X(surface);

	surface_vline(surface, x, y1, y2, RND_COLOR);
}

void test_v_lines_fast(SURFACE *surface) {
	int y1 = RND_TEST_Y(surface);
	int y2 = RND_TEST_Y(surface);
	int x = RND_TEST_X(surface);

	surface_vline_fast(surface, x, y1, y2, RND_COLOR);
}

void test_rects(SURFACE *surface) {
	int x1 = RND_CLIP_TEST_X(surface);
	int x2 = RND_CLIP_TEST_X(surface);
	int y1 = RND_CLIP_TEST_Y(surface);
	int y2 = RND_CLIP_TEST_Y(surface);

	surface_rect(surface, x1, y1, x2, y2, RND_COLOR);
}

void test_rects_fast(SURFACE *surface) {
	int x1 = RND_TEST_X(surface);
	int x2 = RND_TEST_X(surface);
	int y1 = RND_TEST_Y(surface);
	int y2 = RND_TEST_Y(surface);

	surface_rect_fast(surface, x1, y1, x2, y2, RND_COLOR);
}

void test_filled_rects(SURFACE *surface) {
	int x1 = RND_CLIP_TEST_X(surface);
	int x2 = RND_CLIP_TEST_X(surface);
	int y1 = RND_CLIP_TEST_Y(surface);
	int y2 = RND_CLIP_TEST_Y(surface);

	surface_rect_filled(surface, x1, y1, x2, y2, RND_COLOR);
}

void test_filled_rects_fast(SURFACE *surface) {
	int x1 = RND_TEST_X(surface);
	int x2 = RND_TEST_X(surface);
	int y1 = RND_TEST_Y(surface);
	int y2 = RND_TEST_Y(surface);

	surface_rect_filled_fast(surface, x1, y1, x2, y2, RND_COLOR);
}

int main(int argc, char **argv) {
	log_init();
	input_init();
	WINDOW *window = window_init("test_primitives", 1024, 768, 320, 240);
	if (!window)
		return 1;

	srand(time(NULL));

	int step = 0;

	while (step < 12) {
		if (!window_do_events(window))
			break;
		if (input_is_key_pressed(KSYM_ESCAPE))
			break;

		if (input_is_key_pressed(KSYM_SPACE)) {
			surface_clear(window->surface, COLOR_BLACK);
			++step;
		}

		switch (step) {
			case 0:  test_pixels(window->surface); break;
			case 1:  test_pixels_fast(window->surface); break;
			case 2:  test_lines(window->surface); break;
			case 3:  test_lines_fast(window->surface); break;
			case 4:  test_h_lines(window->surface); break;
			case 5:  test_h_lines_fast(window->surface); break;
			case 6:  test_v_lines(window->surface); break;
			case 7:  test_v_lines_fast(window->surface); break;
			case 8:  test_rects(window->surface); break;
			case 9:  test_rects_fast(window->surface); break;
			case 10: test_filled_rects(window->surface); break;
			case 11: test_filled_rects_fast(window->surface); break;
		}

		window_render(window);
	}

	window_destroy(window);
	input_destroy();
	log_end();

	return 0;
}
