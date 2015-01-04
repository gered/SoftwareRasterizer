#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int x;
	int y;
	int width;
	int height;
} RECT;

static inline RECT rect_create(int x, int y, int width, int height) {
	RECT rect;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	return rect;
}

static inline int rect_get_right(const RECT *rect) {
	if (rect->width > 0)
		return rect->x + rect->width - 1;
	else
		return rect->x;
}

static inline int rect_get_bottom(const RECT *rect) {
	if (rect->height > 0)
		return rect->y + rect->height - 1;
	else
		return rect->y;
}

#ifdef __cplusplus
}
#endif
