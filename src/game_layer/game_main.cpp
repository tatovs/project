#include <fstream>

#include <raylib.h>

#include "game_main.h"

struct Game_Data
{
	float x_position = 100.0f;
	float y_position = 100.0f;
}game_data;

bool init_game()
{
	return true;
}

bool update_game()
{
	float delta_time = GetFrameTime();
	if(delta_time > 1.f / 5) {delta_time = 1 / 5.f;}

	if (IsKeyDown(KEY_A)) {game_data.x_position -= 200 * delta_time;}
	if (IsKeyDown(KEY_D)) {game_data.x_position += 200 * delta_time;}
	if (IsKeyDown(KEY_W)) {game_data.y_position -= 200 * delta_time;}
	if (IsKeyDown(KEY_S)) {game_data.y_position += 200 * delta_time;}

	DrawRectangle(game_data.x_position, game_data.y_position, 50, 50, {255, 0, 200, 255});

	return true;
}

void close_game()
{
	std::ofstream f(RESOURCES_PATH "f.text");
	f << "\nCLOSED GAME\n";
	f.close();
}