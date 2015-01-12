#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "vm/vm_stdinc.h"
#include "SDL.h"

typedef struct {
	bool initialized;
	SDL_Window *window;
	int width;
	int height;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	bool fixed_viewport;
	int viewport_width;
	int viewport_height;
	struct SURFACE *surface;
} WINDOW;

WINDOW * window_init(const char *title, int width, int height, int viewport_width, int viewport_height);
bool window_do_events(WINDOW *window);
void window_render(WINDOW *window);
void window_destroy(WINDOW *window);

#ifdef __cplusplus
}
#endif
