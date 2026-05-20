#include <fstream>

#include <raylib.h>

#include "game_main.h"
#include <asserts.h>
#include "asset_manager.h"
#include "game_map.h"
#include "helpres.h"

struct Game_Data
{
	Game_Map game_map;
	Camera2D camera;
}game_data;

Asset_Manager asset_manager;

bool init_game()
{
	asset_manager.load_all();

	game_data.game_map.create(30, 30);

	for (int y = 0; y < game_data.game_map.height; y++)
	{
		for (int x = 0; x < game_data.game_map.width; x++)
		{
			float s = (std::sin(x) + 1.0f) / 2.0f;
			float s1 = (std::sin(x * 0.5) + 1.0f) / 2.0f;

			

			if (game_data.game_map.height - (game_data.game_map.height * 0.3 * s) - (game_data.game_map.height * 0.2 * s1) < y)
			{
				game_data.game_map.get_block_unsafe(x, y).type = Block::dirt;
			}
			else if (x == 0 && y == 0)
			{
				game_data.game_map.get_block_unsafe(0, 0).type = Block::silver;
			}
			else
			{
				game_data.game_map.get_block_unsafe(x, y).type = Block::air;
			}
		}
	}

	game_data.camera.target = {0, 0}; // coordinates at the center of the view.
	game_data.camera.rotation = 0.0f; 
	game_data.camera.zoom = 100.0f; 

	return true;
}

bool update_game()
{
	float delta_time = GetFrameTime();
	if(delta_time > 1.f / 5) {delta_time = 1 / 5.f;}

	game_data.camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};

	ClearBackground({75, 75, 150, 255});

	#pragma region camera movement
	if (IsKeyDown(KEY_LEFT)) {game_data.camera.target.x -= 7.0f * delta_time;}
	if (IsKeyDown(KEY_RIGHT)) {game_data.camera.target.x += 7.0f * delta_time;}
	if (IsKeyDown(KEY_UP)) {game_data.camera.target.y -= 7.0f * delta_time;}
	if (IsKeyDown(KEY_DOWN)) {game_data.camera.target.y += 7.0f * delta_time;}
	#pragma endregion

	BeginMode2D(game_data.camera);

	for (int y = 0; y < game_data.game_map.height; y++)
	{
		for (int x = 0; x < game_data.game_map.width; x++)
		{
			auto &b = game_data.game_map.get_block_unsafe(x, y);

			if (b.type != Block::air)
			{
				DrawTexturePro(
				  asset_manager.textures, // Texture.
					get_texture_atlas(b.type, 0, 32, 32), //source.
					{(float)x, (float)y, 1, 1}, //destination.
					{0, 0}, //origin (top-left corner.
					0.0f,  // angle of rotation clockwise.
					WHITE); // tint.
			}
		}
	}

	EndMode2D();

	return true;
}

void close_game()
{
	std::ofstream f(RESOURCES_PATH "f.text");
	f << "\nCLOSED GAME\n";
	f.close();
}