#include <fstream>

#include <raylib.h>

#include "game_main.h"
#include <asserts.h>
#include <asset_manager.h>

struct Game_Data
{
}game_data;

Asset_Manager asset_manager;

bool init_game()
{
	asset_manager.load_all();
	return true;
}

bool update_game()
{
	float delta_time = GetFrameTime();
	if(delta_time > 1.f / 5) {delta_time = 1 / 5.f;}

	DrawTexturePro(asset_manager.dirt, {0, 0, (float)asset_manager.dirt.width, (float) asset_manager.dirt.height}, {50 , 50, 100, 100}, {}, 0, WHITE);

	return true;
}

void close_game()
{
	std::ofstream f(RESOURCES_PATH "f.text");
	f << "\nCLOSED GAME\n";
	f.close();
}