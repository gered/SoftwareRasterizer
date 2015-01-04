#include <math/rect.h>
#include <fbgfx/surface.h>
#include "fbgfx/surface.h"
#include "fbgfx/clipping.h"
#include "math/rect.h"
#include "types.h"

typedef struct {
	COLOR tint;
} BLIT_PARAMS;

#define BLIT_SOLID_PIXEL(src, dst, src_index, dst_index, params, src_fmt, dst_fmt) \
	source_pixel = GET_PIXEL_IDX_FAST(src_fmt, src, src_index);                    \
	SET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index, source_pixel);

#define BLIT_BLEND_PIXEL(src, dst, src_index, dst_index, params, src_fmt, dst_fmt)      \
	source_pixel = GET_PIXEL_IDX_FAST(src_fmt, src, src_index);                         \
	dest_pixel = GET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index);                           \
	SET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index, color_blend(source_pixel, dest_pixel));

#define BLIT_TRANS_PIXEL(src, dst, src_index, dst_index, params, src_fmt, dst_fmt) \
	source_pixel = GET_PIXEL_IDX_FAST(src_fmt, src, src_index);                    \
	if (color_get_a(source_pixel) > 0)                                             \
		SET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index, source_pixel);

#define BLIT_TINT_PIXEL(src, dst, src_index, dst_index, params, src_fmt, dst_fmt)      \
	source_pixel = GET_PIXEL_IDX_FAST(src_fmt, src, src_index);                        \
	SET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index, color_tint(source_pixel, params.tint));

#define BLIT_TINTALPHA_PIXEL(src, dst, src_index, dst_index, params, src_fmt, dst_fmt) \
	source_pixel = GET_PIXEL_IDX_FAST(src_fmt, src, src_index);                        \
	source_pixel = color_tint_alpha(color_get_a(source_pixel), params.tint);           \
	SET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index, source_pixel);

#define BLIT_BLEND_TINT_PIXEL(src, dst, src_index, dst_index, params, src_fmt, dst_fmt) \
	source_pixel = GET_PIXEL_IDX_FAST(src_fmt, src, src_index);                         \
	dest_pixel = GET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index);                           \
	source_pixel = color_tint(source_pixel, params.tint);                              \
	SET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index, color_blend(source_pixel, dest_pixel));

#define BLIT_BLEND_TINTALPHA_PIXEL(src, dst, src_index, dst_index, params, src_fmt, dst_fmt) \
	source_pixel = GET_PIXEL_IDX_FAST(src_fmt, src, src_index);                              \
	dest_pixel = GET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index);                                \
	source_pixel = color_tint_alpha(color_get_a(source_pixel), params.tint);                 \
	SET_PIXEL_IDX_FAST(dst_fmt, dst, dst_index, color_blend(source_pixel, dest_pixel));

#define BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt)       \
	blit_pixel(src, dst, src_x_index, dst_x_index, params, src_fmt, dst_fmt); \
	src_x_index += src->x_inc;                                                \
	dst_x_index += dst->x_inc;                                                \

#define BLIT(src, dst, src_region, dst_x, dst_y, blit_pixel, params, src_fmt, dst_fmt) \
	do {                                                                               \
		uint32_t src_x_index;                                                          \
		uint32_t src_y_index;                                                          \
		uint32_t dst_x_index;                                                          \
		uint32_t dst_y_index;                                                          \
		COLOR source_pixel;                                                            \
		COLOR dest_pixel;                                                              \
		                                                                               \
		src_y_index = surface_get_index_of(src, src_region.x, src_region.y);           \
		dst_y_index = surface_get_index_of(dst, dst_x, dst_y);                         \
		                                                                               \
		int x;                                                                         \
		int y;                                                                         \
		for (y = 0; y < src_region.height; ++y) {                                      \
			src_x_index = src_y_index;                                                 \
			dst_x_index = dst_y_index;                                                 \
			                                                                           \
			for (x = 0; x < src_region.width; ++x) {                                   \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);       \
			}                                                                          \
			                                                                           \
			src_y_index += src->y_inc;                                                 \
			dst_y_index += dst->y_inc;                                                 \
		}                                                                              \
	} while(0)

#define BLIT_UNROLLED8(src, dst, src_region, dst_x, dst_y, blit_pixel, params, src_fmt, dst_fmt) \
	do {                                                                                         \
		uint32_t src_x_index;                                                                    \
		uint32_t src_y_index;                                                                    \
		uint32_t dst_x_index;                                                                    \
		uint32_t dst_y_index;                                                                    \
		COLOR source_pixel;                                                                      \
		COLOR dest_pixel;                                                                        \
		                                                                                         \
		src_y_index = surface_get_index_of(src, src_region.x, src_region.y);                     \
		dst_y_index = surface_get_index_of(dst, dst_x, dst_y);                                   \
		                                                                                         \
		int x;                                                                                   \
		int y;                                                                                   \
		for (y = 0; y < src_region.height; ++y) {                                                \
			src_x_index = src_y_index;                                                           \
			dst_x_index = dst_y_index;                                                           \
			                                                                                     \
			for (x = 0; x < src_region.width; x += 8) {                                          \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);                 \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);                 \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);                 \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);                 \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);                 \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);                 \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);                 \
				BLIT_INNER_LOOP(src, dst, blit_pixel, params, src_fmt, dst_fmt);                 \
			}                                                                                    \
			                                                                                     \
			src_y_index += src->y_inc;                                                           \
			dst_y_index += dst->y_inc;                                                           \
		}                                                                                        \
	} while(0)

#define BLIT_FMT(src_fmt, dst_fmt) ((src_fmt << 16) | dst_fmt)

#define SELECT_BLIT(src, dst, src_region, dst_x, dst_y, blit, blit_pixel, params)                                                                     \
	switch (BLIT_FMT(src->format, dst->format)) {                                                                                                     \
		case BLIT_FMT(SURFACE_FORMAT_ALPHA, SURFACE_FORMAT_ALPHA): blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, alpha, alpha); break; \
		case BLIT_FMT(SURFACE_FORMAT_ALPHA, SURFACE_FORMAT_RGB):   blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, alpha, rgb);   break; \
		case BLIT_FMT(SURFACE_FORMAT_ALPHA, SURFACE_FORMAT_RGBA):  blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, alpha, rgba);  break; \
		case BLIT_FMT(SURFACE_FORMAT_RGB, SURFACE_FORMAT_ALPHA):   blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, rgb,   alpha); break; \
		case BLIT_FMT(SURFACE_FORMAT_RGB, SURFACE_FORMAT_RGB):     blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, rgb,   rgb);   break; \
		case BLIT_FMT(SURFACE_FORMAT_RGB, SURFACE_FORMAT_RGBA):    blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, rgb,   rgba);  break; \
		case BLIT_FMT(SURFACE_FORMAT_RGBA, SURFACE_FORMAT_ALPHA):  blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, rgba,  alpha); break; \
		case BLIT_FMT(SURFACE_FORMAT_RGBA, SURFACE_FORMAT_RGB):    blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, rgba,  rgb);   break; \
		case BLIT_FMT(SURFACE_FORMAT_RGBA, SURFACE_FORMAT_RGBA):   blit(src, dst, src_region, dst_x, dst_y, blit_pixel, params, rgba,  rgba);  break; \
	}

#define DO_BLIT(src, dst, src_region, dst_x, dst_y, blit_pixel, params)                      \
	if (src_region.width % 8 == 0) {                                                         \
		SELECT_BLIT(src, dst, src_region, dst_x, dst_y, BLIT_UNROLLED8, blit_pixel, params); \
	} else {                                                                                 \
		SELECT_BLIT(src, dst, src_region, dst_x, dst_y, BLIT, blit_pixel, params);           \
	}

void surface_blit_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y) {
	RECT source_region = rect_create(source_x, source_y, source_width, source_height);
	bool on_screen = clip_blit_region(&destination->clip_region, &source_region, &dest_x, &dest_y);
	if (!on_screen)
		return;

	DO_BLIT(source, destination, source_region, dest_x, dest_y, BLIT_SOLID_PIXEL, NULL);
}

void surface_blit_blend_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y) {
	RECT source_region = rect_create(source_x, source_y, source_width, source_height);
	bool on_screen = clip_blit_region(&destination->clip_region, &source_region, &dest_x, &dest_y);
	if (!on_screen)
		return;

	DO_BLIT(source, destination, source_region, dest_x, dest_y, BLIT_BLEND_PIXEL, NULL);
}

void surface_blit_trans_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y) {
	RECT source_region = rect_create(source_x, source_y, source_width, source_height);
	bool on_screen = clip_blit_region(&destination->clip_region, &source_region, &dest_x, &dest_y);
	if (!on_screen)
		return;

	DO_BLIT(source, destination, source_region, dest_x, dest_y, BLIT_TRANS_PIXEL, NULL);
}

void surface_blit_tint_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y, COLOR tint) {
	RECT source_region = rect_create(source_x, source_y, source_width, source_height);
	bool on_screen = clip_blit_region(&destination->clip_region, &source_region, &dest_x, &dest_y);
	if (!on_screen)
		return;

	BLIT_PARAMS params;
	params.tint = tint;

	if (source->format == SURFACE_FORMAT_ALPHA) {
		DO_BLIT(source, destination, source_region, dest_x, dest_y, BLIT_TINTALPHA_PIXEL, params);
	} else {
		DO_BLIT(source, destination, source_region, dest_x, dest_y, BLIT_TINT_PIXEL, params);
	}
}

void surface_blit_blend_tint_region(SURFACE *source, SURFACE *destination, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y, COLOR tint) {
	RECT source_region = rect_create(source_x, source_y, source_width, source_height);
	bool on_screen = clip_blit_region(&destination->clip_region, &source_region, &dest_x, &dest_y);
	if (!on_screen)
		return;

	BLIT_PARAMS params;
	params.tint = tint;

	if (source->format == SURFACE_FORMAT_ALPHA) {
		DO_BLIT(source, destination, source_region, dest_x, dest_y, BLIT_BLEND_TINTALPHA_PIXEL, params);
	} else {
		DO_BLIT(source, destination, source_region, dest_x, dest_y, BLIT_BLEND_TINT_PIXEL, params);
	}
}
