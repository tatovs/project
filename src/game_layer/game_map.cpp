#include "game_map.h"
#include <asserts.h>

void Game_Map::create(int w, int h)
{
	*this = {}; // Reset all data.
	map_data.resize(w * h);
	wall_data.resize(w * h);

	this->width = w;
	this->height = h;

	for (auto &b : map_data) {b = {}; } //Clears all Block data.
	for (auto &w : wall_data) {w = {}; } //Clears all Block data.
}

Block &Game_Map::get_block_unsafe(int x, int y, Game_Map::Layer layer)
{
	permaAssertCommentDevelopement(map_data.size() == width * height, "Map data not initialized");
	permaAssertCommentDevelopement(wall_data.size() == width * height, "Map data not initialized");
	permaAssertCommentDevelopement( x >= 0 && y >= 0 && x < width && y < height, "get:block_unsafe out of bounds error");

	switch (layer)
	{
		case Game_Map::Layer::wall:
			return wall_data[x + y * width];
			break;
		default:
			return map_data[x +y * width];
			break;
	}
}

Block *Game_Map::get_block_safe(int x, int y, Game_Map::Layer  layer)
{
	permaAssertCommentDevelopement(map_data.size() == width * height, "Map data not initialized");
	permaAssertCommentDevelopement(wall_data.size() == width * height, "Map data not initialized");
	
	if(x < 0 || y < 0 || x >= width || y >= height) {return nullptr;}

	switch (layer)
	{
		case Game_Map::Layer::wall:
			return &wall_data[x + y * width];
			break;
		default:
			return &map_data[x + y * width];
			break;
	}
}