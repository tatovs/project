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

	dirt_noise_generator->SetSeed(seed++);

	dirt_noise_generator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	dirt_noise_generator->SetFractalOctaves(4);
	dirt_noise_generator->SetFrequency(0.026);
	
	float *dirt_noise = FastNoiseSIMD::GetEmptySet(width);
	float *dirt_valley_noise = FastNoiseSIMD::GetEmptySet(width);

	dirt_noise_generator->FillNoiseSet(dirt_noise, 0, 0, 0, width, 1, 1);
	
	//convert values from [-1, 1] to [0, 1]
	for (int i = 0; i < width; i++)
	{
		dirt_noise[i] = (dirt_noise[i] +1) / 2;
	}

	int dirt_offset_start = -5;
	int dirt_offset_end = 35;

	int stone_height_start = 80;
	int stone_height_end = 170;

	for (int x = 0; x < width; x++)
	{
		int stone_height = stone_height_start + (stone_height_end - stone_height_start) * 1;
		int dirt_height = dirt_offset_start + (dirt_offset_end - dirt_offset_start) * dirt_noise[x];
		dirt_height = stone_height - dirt_height;

		for (int y = 0; y < height; y++)
		{
			Block b;

			if (y > dirt_height)
			{
				b.type = Block::dirt;
			}
			if (y == dirt_height)
			{
				b.type = Block::grass;
			}
			if (y >= stone_height)
			{
				b.type = Block::stone;
			}
			game_map.get_block_unsafe(x, y) = b;
		}
	}

	FastNoiseSIMD::FreeNoiseSet(dirt_noise);
}