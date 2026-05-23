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