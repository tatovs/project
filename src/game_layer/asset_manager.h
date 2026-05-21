#pragma once

#include <raylib.h>

struct Asset_Manager
{
	Texture2D dirt = {};
	Texture2D textures = {};
	Texture2D frame = {};
	Texture2D trees = {};
	Texture2D textures_walls = {};
	void load_all();
};