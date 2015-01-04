#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../types.h"
#include "../math/rect.h"

bool point_in_bounds(const RECT *clip_region, int x, int y);
bool line_in_bounds(const RECT *clip_region, int x1, int y1, int x2, int y2);
bool clamp_to_region(const RECT *clip_region, int *x1, int *y1, int *x2, int *y2);
bool clip_blit_region(const RECT *dest_clip_region, RECT *source_blit_region, int *dest_x, int *dest_y);

#ifdef __cplusplus
}
#endif
