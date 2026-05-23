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
	enum class Layer
	{
		map = 0,
		wall,

		LAYER_COUNT
	};

	Block &get_block_unsafe(int x, int y, Layer layer = Layer::map);
	Block *get_block_safe(int x, int y, Layer layer = Layer::map);
};