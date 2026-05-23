#include "rng.h"

float get_random_float(std::ranlux24_base &rng, float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}

int get_random_int(std::ranlux24_base &rng, int min, int max)
{
	std::uniform_real_distribution<int> dist(min, max);
	return dist(rng);
}

bool get_random_chance(std::ranlux24_base &rng, float chance)
{
	float dice = get_random_float(rng, 0.0f, 0.1f);
	return dice <= chance;
}

int get_random_texture(std::ranlux24_base &rng, int x_pos, int y_pos, Block type)
{
	uint32_t x = x_pos;
	uint32_t y = y_pos;

	x *= 3266489917 + 374761393;
	x = (x << 17) | (x >> 15);

	x += y * 3266489917;
	
	x *= 668265263;
	x ^= x >> 15;
	x *= 2246822519;
	x ^= x >> 13;
	x *= 3266489917;
	x ^= x >> 16;

	switch (type)
	{
		case Block::door:
			return ((x & 0x00ffffff) * (1.0f / 0x10000000) * 3);
			break;
		default:
		return ((x & 0x00ffffff) * (1.0f / 0x10000000) * 5);
			break;
	}
}