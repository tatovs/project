#pragma once

#include <random>

//random float number betwen min and max, does no include the max. 
float get_random_float(std::ranlux24_base &rng, float min, float max);

//random int number betwen min and max, does no include the max.
int get_random_int(std::ranlux24_base &rng, int min, int max);

bool get_random_chance(std::ranlux24_base &rng, float chacne);