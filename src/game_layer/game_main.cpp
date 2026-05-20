#include <fstream>

#include <raylib.h>
#include <raymath.h>

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
		for (int x = 0; x < game_data.game_map.width; x++)
		{
			game_data.game_map.get_block_unsafe(x, y).type = Block::silver;

			float s = (std::sin(x) + 1.0f) / 2.0f;
			float s1 = (std::sin(x * 0.5) + 1.0f) / 2.0f;

			if (game_data.game_map.height - (game_data.game_map.height * 0.3 * s) - (game_data.game_map.height * 0.2 * s1) < y)
			{
				game_data.game_map.get_block_unsafe(x, y).type = Block::dirt;
			}
			else
			{
				//game_data.game_map.get_block_unsafe(x, y).type = Block::air;
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

	Vector2 world_pos = GetScreenToWorld2D(GetMousePosition(), game_data.camera);
	int x_block = (int)floor(world_pos.x);
	int y_block = (int)floor(world_pos.y);

	#pragma region block selector
	static auto new_b = Block::air;

	if (IsKeyReleased(KEY_ONE)) {new_b = Block::dirt;}
	if (IsKeyReleased(KEY_TWO)) {new_b = Block::wood_plank;}
	if (IsKeyReleased(KEY_THREE)) {new_b = Block::door;}
	if (IsKeyReleased(KEY_FOUR)) {new_b = Block::work_bench;}
	if (IsKeyReleased(KEY_FIVE)) {new_b = Block::jar;}
	if (IsKeyReleased(KEY_SIX)) {new_b = Block::brick;}
	if (IsKeyReleased(KEY_SEVEN)) {new_b = Block::wordrobe;}
	if (IsKeyReleased(KEY_EIGHT)) {new_b = Block::bone_chest;}
	if (IsKeyReleased(KEY_NINE)) {new_b = Block::painting;}
	if (IsKeyReleased(KEY_ZERO)) {new_b = Block::glass;}
	#pragma endregion

	#pragma region mouse editor
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		auto b = game_data.game_map.get_block_safe(x_block, y_block);
		if (b)
		{
			*b = {};
		}
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		auto b = game_data.game_map.get_block_safe(x_block, y_block);
		if (b)
		{
			b->type = new_b;
		}
	}
	#pragma endregion

	#pragma region draw world
	BeginMode2D(game_data.camera);
	
	// Get Corners
	Vector2 top_left_view = GetScreenToWorld2D({0, 0}, game_data.camera);
	Vector2 bottom_right_view = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, game_data.camera);

	//Padding
	int start_x_view = (int)floorf(top_left_view.x - 1);
	int end_x_view = (int)ceilf(bottom_right_view.x + 1);
	int start_y_view = (int)floorf(top_left_view.y - 1);
	int end_y_view = (int)ceilf(bottom_right_view.y + 1);

	//Clamping values within range
	start_x_view = Clamp(start_x_view, 0, game_data.game_map.width - 1);
	end_x_view = Clamp(end_x_view, 0, game_data.game_map.width - 1);
	
	start_y_view = Clamp(start_y_view, 0, game_data.game_map.height - 1);
	end_y_view = Clamp(end_y_view, 0, game_data.game_map.height - 1);

	for (int y = start_y_view; y <= end_y_view; y++)
		for (int x = start_x_view; x <= end_x_view; x++)
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

	//draw frame over selected block.
	DrawTexturePro(
		asset_manager.frame, // Texture.
		{ 0, 0, (float) asset_manager.frame.width, (float) asset_manager.frame.height }, //source.
		{ (float) x_block, (float) y_block, 1, 1 }, //destination.
		{ 0, 0 }, //origin (top-left corner.
		0.0f,  // angle of rotation clockwise.
		WHITE); // tint.

	EndMode2D();
	#pragma endregion

	DrawFPS(10, 10);

	return true;
}

void close_game()
{
	std::ofstream f(RESOURCES_PATH "f.text");
	f << "\nCLOSED GAME\n";
	f.close();
}