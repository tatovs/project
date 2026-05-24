#include "rng.h"

float get_random_float(std::ranlux24_base &rng, float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}

int get_random_int(std::ranlux24_base &rng, int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}

bool get_random_chance(std::ranlux24_base &rng, float chance)
{
	float dice = get_random_float(rng, 0.0f, 0.1f);
	return dice <= chance;
}

int get_random_texture(int x_pos, int y_pos)
{
	uint32_t v = (uint32_t)x_pos * 0x1f1f1f1f ^ (uint32_t)y_pos * 0x5f356495;

	v ^= v >> 16;
	v *= 0x7feb352d;
	v ^= v >> 15;
	v *= 0x846ca68b;
	v ^= v >> 16;

	return v & 3;
}