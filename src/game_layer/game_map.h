#pragma once

#include <vector>

#include <blocks.h>

struct Game_Map
{
	int width = 0;
	int height = 0;

	std::vector<Block> map_data;
  std::vector<Block> wall_data;

	void create(int width, int height);

	Block &get_map_block_unsafe(int x, int y);
	Block *get_map_block_safe(int x, int y);
	Block &get_wall_block_unsafe(int x, int y);
	Block *get_wall_block_safe(int x, int y);
};