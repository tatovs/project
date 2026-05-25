#include "world_generator.h"
#include "rng.h"


void generate_world(Game_Map &game_map, int seed)
{
	const int width = 800;
	const int height = 600;

	game_map.create(width, height);

	std::ranlux24_base rng(seed);

	int dirt_change_cooldown = get_random_int(rng, 5, 35);
	int dirt_change_direction = get_random_int(rng, -3, 3);

	int stone_change_cooldown = get_random_int(rng, 5, 40);
	int stone_change_direction = get_random_int(rng, -2, 2);

	int dirt_height = 60;
	int stone_height = 90;

	for(int x = 0; x < width; x++)
	{
		if (dirt_change_cooldown <= 0)
		{
			dirt_change_cooldown = get_random_int(rng, 5, 35);
			dirt_change_direction = get_random_int(rng, -3, 3);
		}

		if (dirt_change_direction == -3)
		{
			if (get_random_chance(rng, 0.5)) {dirt_height--;}
			if (get_random_chance(rng, 0.5)) {dirt_height--;}
			if (get_random_chance(rng, 0.5)) {dirt_height--;}
		}
		else if (dirt_change_direction == -2)
		{
			if (get_random_chance(rng, 0.25)) {dirt_height--;}
			if (get_random_chance(rng, 0.25)) {dirt_height--;}
		}
		else if (dirt_change_direction == -1)
		{
			if (get_random_chance(rng, 0.25)) {dirt_height--;}
		}
		else if (dirt_change_direction == 1)
		{
			if (get_random_chance(rng, 0.25)) {dirt_height++;}
		}
		else if (dirt_change_direction == 2)
		{
			if (get_random_chance(rng, 0.25)) {dirt_height++;}
			if (get_random_chance(rng, 0.25)) {dirt_height++;}
		}
		else if (dirt_change_direction == 3)
		{
			if (get_random_chance(rng, 0.5)) {dirt_height++;}
			if (get_random_chance(rng, 0.5)) {dirt_height++;}
			if (get_random_chance(rng, 0.5)) {dirt_height++;}
		}
		dirt_change_cooldown--;

		if (stone_change_direction <= 0)
		{
			stone_change_cooldown = get_random_int(rng, 5, 40);
			stone_change_direction = get_random_int(rng, -2, 2);
		}

		if (stone_change_direction == -2)
		{
			if (get_random_chance(rng, 0.35)) {stone_height--;}
			if (get_random_chance(rng, 0.35)) {stone_height--;}
		}
		else if (stone_change_direction == -1)
		{
			if (get_random_chance(rng, 0.35)) {stone_height--;}
		}
		else if (stone_change_direction == 1)
		{
			if (get_random_chance(rng, 0.35)) {stone_height++;}
		}
		else if (stone_change_direction == 2)
		{
			if (get_random_chance(rng, 0.35)) {stone_height++;}
			if (get_random_chance(rng, 0.35)) {stone_height++;}
		}
		stone_change_direction--;

		if (dirt_height < 50)
		{
			dirt_height = 50;
		}
		else if (dirt_height > 90)
		{
			dirt_height = 90;
		}

		if (stone_height < 60)
		{
			stone_height = 60;
		}
		else if (stone_height > 120)
		{
			dirt_height = 120;
		}

		for (int y = 0; y < height; y++)
		{
			Block b;

			if (y == dirt_height)
			{
				b.type = Block::grass_block;
			}
			
			if (y > dirt_height)
			{
				b.type = Block::dirt;
			}

			if(y > stone_height)
			{
				b.type = Block::stone;

				if (get_random_chance(rng, 0.1f))
				{
					b.type = Block::gold;
				}

			}
			game_map.get_block_unsafe(x, y, Game_Map::Layer::map) = b;
		}
	}
}