#include "fbgfx/clipping.h"

bool point_in_bounds(const RECT *clip_region, int x, int y) {
	return (
		x >= clip_region->x &&
		y >= clip_region->y &&
		x <= rect_get_right(clip_region) &&
		y <= rect_get_bottom(clip_region)
		);
}

bool line_in_bounds(const RECT *clip_region, int x1, int y1, int x2, int y2) {
	int clip_region_right = rect_get_right(clip_region);
	int clip_region_bottom = rect_get_bottom(clip_region);

	if (y1 < clip_region->y && 
		y2 < clip_region->y)
		return false;
	if (y1 > clip_region_bottom && 
		y2 > clip_region_bottom)
		return false;
	if (x1 < clip_region->x && 
		x2 < clip_region->x)
		return false;
	if (x1 > clip_region_right && 
		x2 > clip_region_right)
		return false;

	return true;
}

bool clamp_to_region(const RECT *clip_region, int *x1, int *y1, int *x2, int *y2) {
	int clip_region_right = rect_get_right(clip_region);
	int clip_region_bottom = rect_get_bottom(clip_region);

	if (*y1 < clip_region->y && 
		*y2 < clip_region->y)
		return false;
	if (*y1 > clip_region_bottom && 
		*y2 > clip_region_bottom)
		return false;
	if (*x1 < clip_region->x && 
		*x2 < clip_region->x)
		return false;
	if (*x1 > clip_region_right && 
		*x2 > clip_region_right)
		return false;

	if (*y1 < clip_region->y)
		*y1 = clip_region->y;
	if (*y1 > clip_region_bottom)
		*y1 = clip_region_bottom;
	
	if (*y2 < clip_region->y)
		*y2 = clip_region->y;
	if (*y2 > clip_region_bottom)
		*y2 = clip_region_bottom;
	
	if (*x1 < clip_region->x)
		*x1 = clip_region->x;
	if (*x1 > clip_region_right)
		*x1 = clip_region_right;
	
	if (*x2 < clip_region->x)
		*x2 = clip_region->x;
	if (*x2 > clip_region_right)
		*x2 = clip_region_right;

	return true;
}

bool clip_blit_region(const RECT *dest_clip_region, RECT *source_blit_region, int *dest_x, int *dest_y) {
	// check for "completely out of bounds" scenarios first to return early
	if (*dest_x > rect_get_right(dest_clip_region))
		return false;
	if (*dest_x + source_blit_region->width < dest_clip_region->x)
		return false;
	if (*dest_y > rect_get_bottom(dest_clip_region))
		return false;
	if (*dest_y + source_blit_region->height < dest_clip_region->y)
		return false;
	
	// destination region is fully / partially within the clip region
	
	// off the left side?
	if (*dest_x < dest_clip_region->x) {
		int offset = dest_clip_region->x - *dest_x;
		source_blit_region->x += offset;
		source_blit_region->width -= offset;
		*dest_x = dest_clip_region->x;
	}
	
	// off the right side?
	if (*dest_x > (dest_clip_region->width - source_blit_region->width)) {
		int offset = *dest_x + source_blit_region->width - dest_clip_region->width;
		source_blit_region->width -= offset;
	}
	
	// off the top edge?
	if (*dest_y < dest_clip_region->y) {
		int offset = dest_clip_region->y - *dest_y;
		source_blit_region->y += offset;
		source_blit_region->height -= offset;
		*dest_y = dest_clip_region->y;
	}
	
	// off the bottom edge?
	if (*dest_y > (dest_clip_region->height - source_blit_region->height)) {
		int offset = *dest_y + source_blit_region->height - dest_clip_region->height;
		source_blit_region->height -= offset;
	}
	
	return true;
}

