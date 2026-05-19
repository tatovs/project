#pragma once

#include <raylib.h>

struct Asset_Manager
{
	Texture2D dirt = {};

	void load_all();
};