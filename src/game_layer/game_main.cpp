#include <fstream>

#include <raylib.h>

#include "game_main.h"
#include <asserts.h>
#include <asset_manager.h>
#include <game_map.h>

struct Game_Data
{
	Game_Map game_map;
}game_data;

Asset_Manager asset_manager;

bool init_game()
{
	asset_manager.load_all();

	game_data.game_map.create(30, 10);

	game_data.game_map.get_block_unsafe(0, 0).type = Block::dirt;
	game_data.game_map.get_block_unsafe(1, 1).type = Block::dirt;
	game_data.game_map.get_block_unsafe(2, 2).type = Block::dirt;
	game_data.game_map.get_block_unsafe(3, 3).type = Block::dirt;
	game_data.game_map.get_block_unsafe(4, 4).type = Block::dirt;

	return true;
}

bool update_game()
{
	float delta_time = GetFrameTime();
	if(delta_time > 1.f / 5) {delta_time = 1 / 5.f;}

	ClearBackground({75, 75, 150, 255});

	for (int y = 0; y < game_data.game_map.height; y++)
	{
		for (int x = 0; x < game_data.game_map.width; x++)
		{
			auto &b = game_data.game_map.get_block_unsafe(x, y);

			if (b.type != Block::air)
			{
				float size = 32;
				float x_pos = x * size;
				float y_pos = y * size;

				DrawTexturePro(
				  asset_manager.dirt, // Texture.
					Rectangle{0.f, 0.f, (float) asset_manager.dirt.width, (float) asset_manager.dirt.height }, //source.
					{x_pos, y_pos, size, size}, //destination.
					{0, 0}, //origin (top-left corner.
					0.0f,  // angle of rotation clockwise.
					WHITE); // tint.
			}
		}
	}

	return true;
}

void close_game()
{
	std::ofstream f(RESOURCES_PATH "f.text");
	f << "\nCLOSED GAME\n";
	f.close();
}