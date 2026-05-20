#pragma once

#include <cstdint>

struct Block
{
	enum
	{
		air = 0,
		dirt,
		grass_block,
		stone,
		grass,
		sand,
		sand_ruby,
		sand_stone,
		wood_plank,
		stone_brick,
		clay,
		wood_log,
		leaves,
		copper,
		iron,
		gold,
		copper_block,
		iron_block,
		gold_block,
		brick,
		snow,
		ice,
		red_ruby_block,
		wood_platform,
		work_bench,
		glass,
		furnace,
		painting,
		sappling,
		snow_blue_ruby,
		blue_ruby_block,
		door,
		jar,
		table,
		wordrobe,
		book_shelf,
		snow_birck,
		ice_table,
		ice_wordrobe,
		ice_book_shelf,
		ice_platform,
		sand_table,
		sand_wordrobe,
		sand_book_shelf,
		sand_platform,
		wooden_chest,
		ice_chest,
		sand_chest,
		bone_chest,
		bone_brtick,
		bone_table,
		bone_wordrobe,
		bone_book_shelf,
		bone_platform,

		BLOCKS_COUNT,

	};

	std::uint16_t type = 0;
};