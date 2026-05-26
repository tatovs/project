#include "world_generator.h"
#include "rng.h"

#include <FastNoiseSIMD.h>


void generate_world(Game_Map &game_map, int seed)
{
	const int width = 800;
	const int height = 600;

	game_map.create(width, height);

	std::ranlux24_base rng(seed++);

	std::unique_ptr<FastNoiseSIMD> dirt_noise_generator(FastNoiseSIMD::NewFastNoiseSIMD());
	std::unique_ptr<FastNoiseSIMD> stone_noise_generator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirt_noise_generator->SetSeed(seed++);
	stone_noise_generator->SetSeed(seed++);

	dirt_noise_generator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	dirt_noise_generator->SetFractalOctaves(1);
	dirt_noise_generator->SetFrequency(0.02);
	
	stone_noise_generator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	stone_noise_generator->SetFractalOctaves(4);
	stone_noise_generator->SetFrequency(0.01);

	float *dirt_noise = FastNoiseSIMD::GetEmptySet(width);
	float *stone_noise = FastNoiseSIMD::GetEmptySet(width);

	dirt_noise_generator->FillNoiseSet(dirt_noise, 0, 0, 0, width, 1, 1);
	stone_noise_generator->FillNoiseSet(stone_noise, 0, 0, 0, width, 1, 1);
	
	FastNoiseSIMD::FreeNoiseSet(dirt_noise);
	FastNoiseSIMD::FreeNoiseSet(stone_noise);
}