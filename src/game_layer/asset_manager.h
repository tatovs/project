#pragma once

#include <raylib.h>

struct Asset_Manager
{
	Texture2D dirt = {};
	Texture2D textures = {};

	void load_all();
};