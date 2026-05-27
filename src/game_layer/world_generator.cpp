#include "world_generator.h"
#include "rng.h"

#include <FastNoiseSIMD.h>

void generate_world(Game_Map &game_map, int seed)
{
	const int width = 800;
	const int height = 600;

	game_map.create(width, height);

	std::ranlux24_base rng(seed++);
	
	#pragma region noise_setup
	std::unique_ptr<FastNoiseSIMD> dirt_noise_generator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirt_noise_generator->SetSeed(seed++);

	dirt_noise_generator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	dirt_noise_generator->SetFractalOctaves(4);
	dirt_noise_generator->SetFrequency(0.026);
	
	float *dirt_noise = FastNoiseSIMD::GetEmptySet(width);

	dirt_noise_generator->FillNoiseSet(dirt_noise, 0, 0, 0, width, 1, 1);
	
	//convert values from [-1, 1] to [0, 1]
	for (int i = 0; i < width; i++)
	{
		dirt_noise[i] = (dirt_noise[i] +1) / 2;
	}
	#pragma endregion

	#pragma region custom_layer
	int stone_change_cooldown = get_random_int(rng, 5, 20);
	int stone_change_direction = get_random_int(rng, -2, 2);
	#pragma endregion

	int dirt_offset_start = -5;
	int dirt_offset_end = 35;

	int stone_height = 80;

	for (int x = 0; x < width; x++)
	{
		#pragma region stone layer
		stone_change_cooldown--;
		if (stone_change_cooldown <= 0)
		{
			stone_change_cooldown = get_random_int(rng, 5, 20);
			stone_change_direction = get_random_int(rng, -2, 2);
		}

		if (stone_change_direction == -2)
		{
			stone_height--;
			if (get_random_chance(rng, 0.3f)) {stone_height--;}
		}
		else if (stone_change_direction == -1)
		{
			if (get_random_chance(rng, 0.3f)) {stone_height--;}
		}
		else if (stone_change_direction == 1)
		{
			if (get_random_chance(rng, 0.3f)) {stone_height++;}
		}
		else if (stone_change_direction == 2)
		{
			stone_height++;
			if (get_random_chance(rng, 0.3f)) {stone_height++;}
		}

		if (stone_height < 60)
		{
			stone_height = 60;
		}
		else if (stone_height > 120)
		{
			stone_height = 120;
		}
		#pragma endregion
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