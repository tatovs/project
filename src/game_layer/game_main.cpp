#include <fstream>

#include <raylib.h>
#include <raymath.h>
#include<imgui.h>

#include "game_main.h"
#include <asserts.h>
#include "asset_manager.h"
#include "game_map.h"
#include "helpers.h"
#include "rng.h"
#include "world_generator.h"

struct Game_Data
{
	Game_Map game_map;
	Camera2D camera;

	int creative_selected_block = Block::dirt;

}game_data;

Asset_Manager asset_manager;

bool show_imgui = false;

bool init_game()
{
	asset_manager.load_all();

	generate_world(game_data.game_map);

/*
	for (int y = 0; y < game_data.game_map.height; y++)
		for (int x = 0; x < game_data.game_map.width; x++)
		{
			game_data.game_map.get_block_unsafe(x, y, Game_Map::Layer::wall).type = Block::dirt_wall;
		}
*/	

	game_data.camera.target = {20, 120}; // coordinates at the center of the view.
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

	if (IsKeyPressed(KEY_F10)) { show_imgui = !show_imgui; }

	#pragma region camera movement
	static float CAMERA_SPEED = 10.0f;
	if (IsKeyDown(KEY_LEFT)) {game_data.camera.target.x -= CAMERA_SPEED * delta_time;}
	if (IsKeyDown(KEY_RIGHT)) {game_data.camera.target.x += CAMERA_SPEED * delta_time;}
	if (IsKeyDown(KEY_UP)) {game_data.camera.target.y -= CAMERA_SPEED * delta_time;}
	if (IsKeyDown(KEY_DOWN)) {game_data.camera.target.y += CAMERA_SPEED * delta_time;}
	#pragma endregion

	Vector2 world_pos = GetScreenToWorld2D(GetMousePosition(), game_data.camera);
	int x_block = (int) floor(world_pos.x);
	int y_block = (int) floor(world_pos.y);

	#pragma region block selector
	if (game_data.creative_selected_block < 0) { game_data.creative_selected_block = 0; }
	if (game_data.creative_selected_block >= Block::BLOCKS_COUNT) { game_data.creative_selected_block = Block::BLOCKS_COUNT - 1; }
	#pragma endregion

	#pragma region mouse editor
	if (!show_imgui)
	{
		if (IsKeyDown(KEY_LEFT_CONTROL))
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				auto b = game_data.game_map.get_block_safe(x_block, y_block, Game_Map::Layer::map);
				if (b)
				{
					game_data.creative_selected_block = b->type;
				}
			}
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			if (IsKeyDown(KEY_NULL))
			{
				auto b = game_data.game_map.get_block_safe(x_block, y_block, Game_Map::Layer::map);
				if (b)
				{
					*b = {};
				}
			}
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			auto b = game_data.game_map.get_block_safe(x_block, y_block, Game_Map::Layer::map);
			if (b)
			{
				b->type = game_data.creative_selected_block;
			}
		}
	}
	#pragma endregion

	#pragma region draw world
	BeginMode2D(game_data.camera);
	
	//Get Corners.
	Vector2 top_left_view = GetScreenToWorld2D({0, 0}, game_data.camera);
	Vector2 bottom_right_view = GetScreenToWorld2D({(float)GetScreenWidth(), (float)GetScreenHeight()}, game_data.camera);

	//Padding.
	int start_x_view = (int)floorf(top_left_view.x - 1);
	int end_x_view = (int)ceilf(bottom_right_view.x + 1);
	int start_y_view = (int)floorf(top_left_view.y - 1);
	int end_y_view = (int)ceilf(bottom_right_view.y + 1);

	//Clamping values within range.
	start_x_view = Clamp(start_x_view, 0, game_data.game_map.width - 1);
	end_x_view = Clamp(end_x_view, 0, game_data.game_map.width - 1);
	
	start_y_view = Clamp(start_y_view, 0, game_data.game_map.height - 1);
	end_y_view = Clamp(end_y_view, 0, game_data.game_map.height - 1);

	//Draw walls inside padding area.
	for (int y = start_y_view; y <= end_y_view; y++)
		for (int x = start_x_view; x <= end_x_view; x++)
		{
			auto &w = game_data.game_map.get_block_unsafe(x, y, Game_Map::Layer::wall);
			int variant = get_random_texture(x, y);

			if (w.type != Block::air)
			{
				DrawTexturePro(
				asset_manager.textures_walls, // Texture.
				get_texture_atlas(w.type, variant, 32, 32), //source.
				{(float) x, (float) y, 1, 1}, //destination.
				{0, 0}, //origin (top-left corner.
				0.0f,  // angle of rotation clockwise.
				WHITE); // tint.
			}
		}

	//Draw blocks inside padding area.
	for (int y = start_y_view; y <= end_y_view; y++)
		for (int x = start_x_view; x <= end_x_view; x++)
		{
			auto &b = game_data.game_map.get_block_unsafe(x, y, Game_Map::Layer::map);
			int variant = get_random_texture(x, y);

			if (b.type == Block::wood_log)
			{
				auto up_b = !game_data.game_map.get_block_safe(x, y - 1, Game_Map::Layer::map) ? game_data.game_map.get_block_safe(x, y, Game_Map::Layer::map) : game_data.game_map.get_block_safe(x, y - 1, Game_Map::Layer::map);
				auto down_b = !game_data.game_map.get_block_safe(x, y + 1, Game_Map::Layer::map) ? game_data.game_map.get_block_safe(x, y, Game_Map::Layer::map) : game_data.game_map.get_block_safe(x, y + 1, Game_Map::Layer::map);
				auto left_b = !game_data.game_map.get_block_safe(x - 1, y, Game_Map::Layer::map) ? game_data.game_map.get_block_safe(x, y, Game_Map::Layer::map) : game_data.game_map.get_block_safe(x - 1, y, Game_Map::Layer::map);
				auto right_b = !game_data.game_map.get_block_safe(x + 1, y, Game_Map::Layer::map) ? game_data.game_map.get_block_safe(x, y, Game_Map::Layer::map) : game_data.game_map.get_block_safe(x + 1, y, Game_Map::Layer::map);
				int x_wood = 0;

				if (up_b->type == Block::leaves && down_b->type == Block::wood_log) {x_wood = 5;}
				else if (down_b->type != Block::wood_log && down_b->type != Block::leaves) {up_b->type == Block::wood_log ? x_wood = 4 : x_wood = 7;}
				else if (left_b->type == Block::leaves && right_b->type == Block::leaves && down_b->type == Block::wood_log) {x_wood = 1;}
				else if (left_b->type != Block::leaves && right_b->type == Block::leaves && down_b->type == Block::wood_log) {x_wood = 2;}
				else if (left_b->type == Block::leaves && right_b->type != Block::leaves && down_b->type == Block::wood_log) {x_wood = 3;}
				else if (up_b->type != Block::leaves && down_b->type == Block::wood_log) {up_b->type == Block::wood_log ? x_wood = 0 : x_wood = 6;}

				DrawTexturePro(
					asset_manager.trees, // Texture.
					get_texture_atlas(x_wood, variant, 32, 32), //source.
					{(float)x, (float)y, 1, 1}, //destination.
					{0, 0}, //origin (top-left corner.
					0.0f,  // angle of rotation clockwise.
					WHITE); // tint.
			}
			else if (b.type != Block::air)
			{
				DrawTexturePro(
				asset_manager.textures, // Texture.
				get_texture_atlas(b.type, variant, 32, 32), //source.
				{(float)x, (float)y, 1, 1}, //destination.
				{0, 0}, //origin (top-left corner.
				0.0f,  // angle of rotation clockwise.
				WHITE); // tint.
			}
		}

	//Draw Current Selected Block.
	DrawTexturePro(
		asset_manager.textures_walls, // Texture.
		get_texture_atlas(game_data.creative_selected_block, 0, 32, 32), //source.
		{(float)x_block, (float)y_block, 1, 1}, //destination.
		{0, 0}, //origin (top-left corner.
		0.0f,  // angle of rotation clockwise.
		Fade(WHITE, 0.75f)); // tint.

	EndMode2D();
	#pragma endregion

	if (show_imgui)
	{
		ImGui::Begin("Game Controls");
		ImGui::SliderFloat("Camera Zoom:", &game_data.camera.zoom, 10, 150);
		ImGui::SliderFloat("Camera Speed:", &CAMERA_SPEED, 5, 30);

		ImGui::Separator();

		for (int i = 0; i < Block::BLOCKS_COUNT; i++)
		{
			auto atlas = get_texture_atlas(i, 0, 32, 32);
			atlas.x /= asset_manager.textures.width;
			atlas.width /= asset_manager.textures.width;
			atlas.y /= asset_manager.textures.height;
			atlas.height /= asset_manager.textures.height;

			ImGui::PushID(i);

			ImTextureID tex = (ImTextureID)(intptr_t)asset_manager.textures.id;

			if (ImGui::ImageButton(" ", tex, {35, 35}, {atlas.x, atlas.y}, {atlas.x + atlas.width, atlas.y + atlas.height}))
			{
				game_data.creative_selected_block = i;
			}

			ImGui::PopID();

			if (i % 10 != 0)
			{
				ImGui::SameLine();
			}
		}

		ImGui::End();
	}

	DrawFPS(10, 10);

	return true;
}

void close_game()
{
	std::ofstream f(RESOURCES_PATH "f.text");
	f << "\nCLOSED GAME\n";
	f.close();
}