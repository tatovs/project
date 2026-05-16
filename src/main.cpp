#include <iostream>

#include <raylib.h>
#include <imgui.h>

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "test");

	if (!IsWindowReady)
	{
		TraceLog(LOG_ERROR, "Couldn't Initialize raylib Window.");
	}

	SetTargetFPS(60);
	int x_pos = 30;
	int y_pos = 30;
	int size = 100;

	while (!WindowShouldClose())
	{
		PollInputEvents();


		/*
			Drawing Logic
		*/
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("Testing Text Drawing", 5, 5, 20, WHITE);
		DrawRectangle(x_pos, y_pos, size, size, RED);

		DrawRectangle(75, 75, size, size, { 0, 255, 0, 255 });
		DrawRectangle(50, 50, size, size, {255, 0, 0, 255});
		

		if (x_pos >= 800)
		{
			x_pos = 0;
		}

		x_pos++;

		EndDrawing();

	}

	/*
		Cleanup
	*/
	CloseWindow ();

	return 0;
}