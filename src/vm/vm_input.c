#include "vm/vm_input.h"
#include "vm/vm_log.h"
#include <string.h>

const int MAX_KEYS = SDL_NUM_SCANCODES;
const int MAX_MOUSE_BUTTONS = 3;

struct {
	bool keys[MAX_KEYS];
	bool locked_keys[MAX_KEYS];
	bool mouse_buttons[MAX_MOUSE_BUTTONS];
	bool locked_mouse_buttons[MAX_MOUSE_BUTTONS];
	uint mouse_x;
	uint mouse_y;
	int mouse_delta_x;
	int mouse_delta_y;
} input_state;

bool input_init() {
	LOG_INFO("input_init\n");

	memset(input_state.keys, 0, sizeof(bool) * MAX_KEYS);
	memset(input_state.locked_keys, 0, sizeof(bool) * MAX_KEYS);
	memset(input_state.mouse_buttons, 0, sizeof(bool) * MAX_MOUSE_BUTTONS);
	memset(input_state.locked_mouse_buttons, 0, sizeof(bool) * MAX_MOUSE_BUTTONS);
	input_state.mouse_x = 0;
	input_state.mouse_y = 0;
	input_state.mouse_delta_x = 0;
	input_state.mouse_delta_y = 0;

	return true;
}

void input_destroy() {
	LOG_INFO("input_destroy\n");
}

void input_on_key_event(SDL_Event *event) {
	if (event->type == SDL_KEYUP) {
		int key = event->key.keysym.scancode;
		input_state.keys[key] = false;
		input_state.locked_keys[key] = false;
		
	} else if (event->type == SDL_KEYDOWN) {
		int key = event->key.keysym.scancode;
		input_state.keys[key] = !(input_state.locked_keys[key]);
	}
}

void input_on_mouse_event(SDL_Event *event) {
	int button;
	
	switch (event->type) {
		case SDL_MOUSEMOTION:
			input_state.mouse_x = event->motion.x;
	        input_state.mouse_y = event->motion.y;
	        input_state.mouse_delta_x = event->motion.xrel;
	        input_state.mouse_delta_y = event->motion.yrel;
			break;
			
		case SDL_MOUSEBUTTONUP:
			button = event->button.button;
	        input_state.mouse_buttons[button] = false;
	        input_state.locked_mouse_buttons[button] = false;
			break;
			
		case SDL_MOUSEBUTTONDOWN:
			button = event->button.button;
	        input_state.mouse_buttons[button] = !(input_state.locked_mouse_buttons[button]);
			break;
	}
}

bool input_is_key_down(KEYS key) {
	SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
	return input_state.keys[scancode] && !input_state.locked_keys[scancode];
}

bool input_is_key_pressed(KEYS key) {
	SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
	if (input_state.keys[scancode] && !input_state.locked_keys[scancode]) {
		input_state.locked_keys[scancode] = true;
		return true;
	} else
		return false;
}

void input_lock_key(KEYS key) {
	SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
	input_state.locked_keys[scancode] = true;
}

bool input_is_mouse_button_down(MOUSE_BUTTONS button) {
	return input_state.mouse_buttons[button] && !input_state.locked_mouse_buttons[button];
}

bool input_is_mouse_button_pressed(MOUSE_BUTTONS button) {
	if (input_state.mouse_buttons[button] && !input_state.locked_mouse_buttons[button]) {
		input_state.locked_mouse_buttons[button] = true;
		return true;
	} else
		return false;
}

void input_lock_mouse_button(MOUSE_BUTTONS button) {
	input_state.locked_mouse_buttons[button] = true;
}

int input_get_mouse_delta_x() {
	return input_state.mouse_delta_x;
}

int input_get_mouse_delta_y() {
	return input_state.mouse_delta_y;
}

uint input_get_mouse_x() {
	return input_state.mouse_x;
}

uint input_get_mouse_y() {
	return input_state.mouse_y;
}
