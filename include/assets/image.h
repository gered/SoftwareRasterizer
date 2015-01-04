#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../fbgfx/surface.h"
#include "../types.h"

SURFACE* image_load_file(const char *filename);
SURFACE* image_load_memory(const void *image_file_bin, uint32_t size);

#ifdef __cplusplus
}
#endif
