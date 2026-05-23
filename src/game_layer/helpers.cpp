#include "helpers.h"

Rectangle get_texture_atlas(int x, int y, int cell_size_pixels_x, int cell_size_pixels_y)
{
	return Rectangle{(float)x * cell_size_pixels_x, (float)y * cell_size_pixels_y, (float)cell_size_pixels_x, (float)cell_size_pixels_y};
}