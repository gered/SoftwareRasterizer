#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"

typedef enum {
	MOUSE_LEFT = 1,
	MOUSE_MIDDLE = 2,
	MOUSE_RIGHT = 3
} MOUSE_BUTTONS;

// the idea behind this enum is that on other platforms (that may or may not use SDL)
// we could just remap to the platform's API, but use the same key constants

typedef enum {
	KSYM_UNKNOWN            = 0,
	KSYM_ESCAPE             = SDLK_ESCAPE,
	KSYM_BACKSPACE          = SDLK_BACKSPACE,
	KSYM_END                = SDLK_END,
	KSYM_SOFT_LEFT          = 0,
	KSYM_SOFT_RIGHT         = 0,
	KSYM_HOME               = SDLK_HOME,
	KSYM_BACK               = 0,
	KSYM_CALL               = 0,
	KSYM_ENDCALL            = 0,
	KSYM_0                  = SDLK_0,
	KSYM_1                  = SDLK_1,
	KSYM_2                  = SDLK_2,
	KSYM_3                  = SDLK_3,
	KSYM_4                  = SDLK_4,
	KSYM_5                  = SDLK_5,
	KSYM_6                  = SDLK_6,
	KSYM_7                  = SDLK_7,
	KSYM_8                  = SDLK_8,
	KSYM_9                  = SDLK_9,
	KSYM_ASTERISK           = SDLK_ASTERISK,
	KSYM_HASH               = SDLK_HASH,
	KSYM_UP                 = SDLK_UP,
	KSYM_DOWN               = SDLK_DOWN,
	KSYM_LEFT               = SDLK_LEFT,
	KSYM_RIGHT              = SDLK_RIGHT,
	KSYM_DPAD_CENTER        = 0,
	KSYM_VOLUME_UP          = 0,
	KSYM_VOLUME_DOWN        = 0,
	KSYM_CAMERA             = 0,
	KSYM_CLEAR              = 0,
	KSYM_A                  = SDLK_a,
	KSYM_B                  = SDLK_b,
	KSYM_C                  = SDLK_c,
	KSYM_D                  = SDLK_d,
	KSYM_E                  = SDLK_e,
	KSYM_F                  = SDLK_f,
	KSYM_G                  = SDLK_g,
	KSYM_H                  = SDLK_h,
	KSYM_I                  = SDLK_i,
	KSYM_J                  = SDLK_j,
	KSYM_K                  = SDLK_k,
	KSYM_L                  = SDLK_l,
	KSYM_M                  = SDLK_m,
	KSYM_N                  = SDLK_n,
	KSYM_O                  = SDLK_o,
	KSYM_P                  = SDLK_p,
	KSYM_Q                  = SDLK_q,
	KSYM_R                  = SDLK_r,
	KSYM_S                  = SDLK_s,
	KSYM_T                  = SDLK_t,
	KSYM_U                  = SDLK_u,
	KSYM_V                  = SDLK_v,
	KSYM_W                  = SDLK_w,
	KSYM_X                  = SDLK_x,
	KSYM_Y                  = SDLK_y,
	KSYM_Z                  = SDLK_z,
	KSYM_COMMA              = SDLK_COMMA,
	KSYM_PERIOD             = SDLK_PERIOD,
	KSYM_LALT               = SDLK_LALT,
	KSYM_RALT               = SDLK_RALT,
	KSYM_RCTRL              = SDLK_RCTRL,
	KSYM_LCTRL              = SDLK_LCTRL,
	KSYM_LSHIFT             = SDLK_LSHIFT,
	KSYM_RSHIFT             = SDLK_RSHIFT,
	KSYM_TAB                = SDLK_TAB,
	KSYM_SPACE              = SDLK_SPACE,
	KSYM_SYM                = 0,
	KSYM_RETURN             = SDLK_RETURN,
	KSYM_DELETE             = SDLK_DELETE,
	KSYM_MINUS              = SDLK_MINUS,
	KSYM_EQUALS             = SDLK_EQUALS,
	KSYM_LEFTBRACKET        = SDLK_LEFTBRACKET,
	KSYM_RIGHTBRACKET       = SDLK_RIGHTBRACKET,
	KSYM_BACKSLASH          = SDLK_BACKSLASH,
	KSYM_SEMICOLON          = SDLK_SEMICOLON,
	KSYM_QUOTE              = SDLK_QUOTE,
	KSYM_SLASH              = SDLK_SLASH,
	KSYM_AT                 = SDLK_AT,
	KSYM_PLUS               = SDLK_PLUS,
	KSYM_MENU               = 0,
	KSYM_SEARCH             = 0,
	KSYM_PAGEUP             = SDLK_PAGEUP,
	KSYM_PAGEDOWN           = SDLK_PAGEDOWN
} KEYS;

#ifdef __cplusplus
}
#endif
