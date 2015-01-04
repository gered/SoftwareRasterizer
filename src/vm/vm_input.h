#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "vm_stdinc.h"
#include "vm_input_codes.h"
#include "SDL.h"

bool input_init();
void input_destroy();

void input_on_key_event(SDL_Event *event);
void input_on_mouse_event(SDL_Event *event);

bool input_is_key_down(KEYS key);
bool input_is_key_pressed(KEYS key);
void input_lock_key(KEYS key);
bool input_is_mouse_button_down(MOUSE_BUTTONS button);
bool input_is_mouse_button_pressed(MOUSE_BUTTONS button);
void input_lock_mouse_button(MOUSE_BUTTONS button);
int input_get_mouse_delta_x();
int input_get_mouse_delta_y();
uint input_get_mouse_x();
uint input_get_mouse_y();

#ifdef __cplusplus
}
#endif
