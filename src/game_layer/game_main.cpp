#include <raylib.h>

#include "game_main.h"

bool init_game()
{
	return true;
}

bool update_game()
{
	Color c;
	c.r = 255;
	c.g = 0;
	c.b = 200;
	c.a = 255;

	DrawText("Congrats! You created your first window!", 190, 200, 20, c);
	return true;
}

void close_game()
{

}