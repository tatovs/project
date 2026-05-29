#include "world_generator.h"
#include "rng.h"

#include <FastNoiseSIMD.h>

void generate_world(Game_Map &game_map, int seed)
{
	const int width = 900;
	const int height = 600;

	game_map.create(width, height);

	std::ranlux24_base rng(seed++);
	
	#pragma region deset biome setup
	int desert_start = get_random_int(rng, 10, width - 210);
	int desert_end = desert_start + 100 + get_random_int(rng, 0, 100);

	if (desert_end > width) {desert_end = width;}
	#pragma endregion
	#pragma region dirt_noise_setup
	std::unique_ptr<FastNoiseSIMD> dirt_noise_generator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirt_noise_generator->SetSeed(seed++);

	dirt_noise_generator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	dirt_noise_generator->SetFractalOctaves(4);
	dirt_noise_generator->SetFractalGain(0.4f);
	dirt_noise_generator->SetFrequency(0.026);
	
	float *dirt_noise = FastNoiseSIMD::GetEmptySet(width);

	dirt_noise_generator->FillNoiseSet(dirt_noise, 0, 0, 0, width, 1, 1);
	
	//convert values from [-1, 1] to [0, 1]
	for (int i = 0; i < width; i++)
	{
		dirt_noise[i] = (dirt_noise[i] +1) / 2;
	}
	#pragma endregion
	#pragma region stone_layer_setup
	int stone_change_cooldown = get_random_int(rng, 5, 28);
	int stone_change_direction = get_random_int(rng, -2, 2);
	#pragma endregion
	#pragma region cave noise setup
	std::unique_ptr<FastNoiseSIMD> cave_noise_generator(FastNoiseSIMD::NewFastNoiseSIMD());

	cave_noise_generator->SetSeed(seed++);

	cave_noise_generator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	cave_noise_generator->SetFractalOctaves(3);
	cave_noise_generator->SetFrequency(0.02f);

	float *cave_noise = FastNoiseSIMD::GetEmptySet(width * height);

	cave_noise_generator->FillNoiseSet(cave_noise, 0, 0, 0, height, width, 1);

	//convert values from [-1, 1] to [0, 1]
	for (int i = 0; i < width * height; i++)
	{
		cave_noise[i] = (cave_noise[i] + 1) / 2;
	}

	auto get_cave_noise = [&] (int x, int y)
		{
			return cave_noise[x + y * width];
		};
	#pragma endregion

	int dirt_offset_start = -5;
	int dirt_offset_end = 35;

	int stone_height = 80;

	for (int x = 0; x < width; x++)
	{
		int dirt_type = Block::dirt;
		int grass_type = Block::grass;
		int stone_type = Block::stone;

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
		#pragma region dirt layer
		int dirt_height = dirt_offset_start + (dirt_offset_end - dirt_offset_start) * dirt_noise[x];
		dirt_height = stone_height - dirt_height;
		#pragma endregion
		#pragma region desert biome
		bool is_desert = (x >= desert_start && x <= desert_end);

		if (is_desert)
		{
			dirt_type = Block::sand;
			grass_type = Block::sand;
			stone_type = Block::sand_stone;
		}
		#pragma endregion

		for (int y = 0; y < height; y++)
		{
			Block b;

			if (y > dirt_height) {b.type = dirt_type;}
			if (y == dirt_height) {b.type = grass_type;}
			if (y >= stone_height) {b.type = stone_type;}
			if (is_desert)
			{
				int desert_center_point = (desert_start + desert_end) / 2;
				int desert_half_width = (desert_end - desert_start) / 2;
				int distance_from_center = std::abs(x - desert_center_point);
				float desert_distance = 1 - distance_from_center / float(desert_half_width);
				int desert_stone_start = 10 + stone_height;
				int desert_stone_depth = 20 + stone_height;
				int desert_stone_triangle = desert_stone_start + desert_distance * desert_stone_depth;

				if (y > desert_stone_triangle) {b.type = Block::stone;}
			}
			if (get_cave_noise(x, y) < 0.30) {b.type = Block::air;}
			game_map.get_block_unsafe(x, y) = b;
		}
	}

	FastNoiseSIMD::FreeNoiseSet(cave_noise);
	FastNoiseSIMD::FreeNoiseSet(dirt_noise);

	#pragma region perlin worms
	for (size_t worm = 0; worm < 1; worm++)
	{
		//Start position.
		float x = get_random_float(rng, 0, width - 0);
		float y = get_random_float(rng, 0, height - 0);

		//Time worm is alive and initial direction of movement.
		int worm_life = get_random_int(rng, 50, 500);

		float worm_x_direction = get_random_float(rng, -1, 1);
		float worm_y_direction = get_random_float(rng, -1, 1);

		float worm_radius = 1.0f;

		int worm_direction_cooldown = get_random_int(rng, 5, 25);

		for (int cycle = 0; cycle < worm_life; cycle++)
		{
			for (int x_radius = -worm_radius; x_radius <= worm_radius; x_radius++)
			{
				for (int y_radius = - worm_radius; y_radius <= worm_radius; y_radius++)
				{
				auto b = game_map.get_block_safe(x, y, Game_Map::Layer::map);
				if (b) {b->type = Block::blue_ruby_block;}
				}
			}
			worm_direction_cooldown--;
			if (worm_direction_cooldown <= 0)
			{
				worm_direction_cooldown = get_random_int(rng, 5, 25);
				worm_x_direction = get_random_float(rng, -1, 1);
				worm_y_direction = get_random_float(rng, -1, 1);
			}
			x += worm_x_direction * 1.5f;
			y += worm_y_direction * 1.5f;
		}
	}
	#pragma endregion
}