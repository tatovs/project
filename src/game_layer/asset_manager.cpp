#include "asset_manager.h"

void Asset_Manager::load_all()
{
	dirt = LoadTexture(RESOURCES_PATH "dirt.png");
	textures = LoadTexture(RESOURCES_PATH "textures.png");
	frame = LoadTexture(RESOURCES_PATH "frame.png");
	trees = LoadTexture(RESOURCES_PATH "treetextures.png");
	textures_walls = LoadTexture(RESOURCES_PATH "texturesWithBackgroundVersion.png");
}