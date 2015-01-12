#include "vm/vm_window.h"
#include "vm/vm_log.h"
#include "vm/vm_input.h"
#include "fbgfx/surface.h"

static SDL_Texture*create_texture(SDL_Renderer *renderer, int width, int height) {
	return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, width, height);
}

static bool resize_texture(SDL_Renderer *renderer, SDL_Texture **texture, int width, int height) {
	if (*texture)
		SDL_DestroyTexture(*texture);
	*texture = create_texture(renderer, width, height);
	if (*texture == NULL) {
		LOG_ERROR("Failed to resize texture to %dx%d\n", width, height);
		return false;
	}
	
	return true;
}

static bool resize_window(WINDOW *window, int width, int height) {
	if (window->fixed_viewport)
		return true;
	
	LOG_INFO("Resizing window to %dx%d\n", width, height);
	window->width = width;
	window->height = height;
	bool result = resize_texture(window->renderer, &window->texture, width, height);
	if (result)
		surface_resize(window->surface, width, height);
	
	return result;
}

WINDOW * window_init(const char *title, int width, int height, int viewport_width, int viewport_height) {
	LOG_INFO("window_init(%s, %d, %d, %d, %d)\n", title, width, height, viewport_width, viewport_height);

	WINDOW *window = malloc(sizeof(WINDOW));
	window->surface = NULL;
	
	LOG_INFO("Initializing SDL.\n");
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		LOG_ERROR("SDL_Init error: %s\n", SDL_GetError());
		window_destroy(window);
		return NULL;
	}
	
	window->initialized = true;
	
	window->width = width;
	window->height = height;

	LOG_INFO("Creating a %dx%d window.\n", window->width, window->height);
	window->window = SDL_CreateWindow(title, 
	                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
	                                  window->width, window->height, 
	                                  SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (window->window == NULL) {
		LOG_ERROR("SDL_CreateWindow error: %s\n", SDL_GetError());
		window_destroy(window);
		return NULL;
	}
		
	LOG_INFO("Creating an SDL renderer.\n");
	window->renderer = SDL_CreateRenderer(window->window, -1, 0);
	if (window->renderer == NULL) {
		LOG_ERROR("SDL_CreateRenderer error: %s\n", SDL_GetError());
		window_destroy(window);
		return NULL;
	}
	
	if (viewport_width != 0 && viewport_height != 0) {
		LOG_INFO("Using fixed viewport of %dx%d\n", viewport_width, viewport_height);
		window->fixed_viewport = true;
		window->viewport_width = viewport_width;
		window->viewport_height = viewport_height;
		
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
		SDL_RenderSetLogicalSize(window->renderer, window->viewport_width, window->viewport_height);
	} else {
		LOG_INFO("Using automatically sized viewport (always match window dimensions)\n");
		window->fixed_viewport = false;
		window->viewport_width = width;
		window->viewport_height = height;
	}

	LOG_INFO("Creating a %dx%d texture for the 2D framebuffer.\n", window->viewport_width, window->viewport_height);
	window->texture = create_texture(window->renderer, window->viewport_width, window->viewport_height);
	if (window->texture == NULL) {
		LOG_ERROR("SDL_CreateTexture error: %s\n", SDL_GetError());
		window_destroy(window);
		return NULL;
	}

	window->surface = surface_create(window->viewport_width, window->viewport_height, SURFACE_FORMAT_RGB, SURFACE_FLAGS_NONE);
	
	return window;
}

bool window_do_events(WINDOW *window) {
	bool quit = false;
	SDL_Event e;
	
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				LOG_INFO("Event: SDL_QUIT\n");
				quit = true;
				break;
				
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
					LOG_INFO("Event: SDL_WINDOWEVENT_RESIZED\n");
					if (!resize_window(window, e.window.data1, e.window.data2))
						quit = true;
				}
				break;
				
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				input_on_key_event(&e);
				break;
				
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				input_on_mouse_event(&e);
				break;
		}
	}

	return !quit;
}

void window_render(WINDOW *window) {
	// HACK: there seems to be either some weird behaviour i'm just not understanding or a bug somewhere in
	//       either SDL or my own code that sets up these window / surface dimensions...
	//       basically, the "pass a NULL SDL_Rect to specify the entire surface region" does not seem to
	//       work as expected. if I use NULL's for all those arguments below, then I get a full-width
	//       but half-ish height (yes, not exactly half... seems random...) texture being displayed.
	//       eeerrrrggghh! wtf!?
	
	SDL_Rect src_region;
	src_region.x = 0;
	src_region.y = 0;
	src_region.w = window->surface->width;
	src_region.h = window->surface->height;
	
	SDL_Rect dest_region;
	dest_region.x = 0;
	dest_region.y = 0;
	dest_region.w = window->width;
	dest_region.h = window->height;
	
	SDL_UpdateTexture(window->texture, NULL, (void*)window->surface->pixels, window->surface->width * window->surface->bytes_per_pixel);
	
	SDL_RenderClear(window->renderer);
//	SDL_RenderCopy(window->renderer, window->texture, &src_region, &dest_region);
	SDL_RenderCopy(window->renderer, window->texture, NULL, NULL);
	SDL_RenderPresent(window->renderer);
}

void window_destroy(WINDOW *window) {
	LOG_INFO("window_destroy\n");

	surface_destroy(window->surface);
	
	if (window->texture) {
		LOG_INFO("Destroying 2D framebuffer texture.\n");
		SDL_DestroyTexture(window->texture);
	}
	
	if (window->renderer) {
		LOG_INFO("Destroying SDL renderer.\n");
		SDL_DestroyRenderer(window->renderer);
	}
	
	if (window->window) {
		LOG_INFO("Destroying window.\n");
		SDL_DestroyWindow(window->window);
	}
	
	if (window->initialized) {
		LOG_INFO("SDL_Quit\n");
		SDL_Quit();
	}

	free(window);
}
