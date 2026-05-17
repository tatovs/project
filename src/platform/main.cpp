#include <iostream>

#include <raylib.h>
#include <imgui.h>
#include <rlimgui/rlImGui.h>

#include <game_main.h>

int main()
{
	#if PRODUCTION_BUILD == 1
	SetTraceLogLevel(LOG_NONE);
	#else
	SetTraceLogLevel(LOG_ALL);
	#endif

	#pragma region initial setup
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "test");
	SetExitKey(KEY_NULL); // Disable "Esc" key from closing the window.
	SetTargetFPS(240);

	if (!IsWindowReady())
	{
		TraceLog(LOG_ERROR, "Couldn't Initialize raylib Window.");
	}
	#pragma endregion

	#pragma region global imgui setup
	rlImGuiSetup(true);

	ImGuiIO &io = ImGui::GetIO();
	//io.ConfigFlags | ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Control
	//io.ConfigFlags | ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Control
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking.
	io.FontGlobalScale = 1; // Adjust Font scaling.
	#pragma endregion

	if (!init_game())
	{
		return 0;
	}

	while (!WindowShouldClose())
	{

		/*
			Drawing Logic
		*/
		BeginDrawing();
		ClearBackground(BLACK);
		rlImGuiBegin();

		#pragma region ImGui_docking
			ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
			ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, ImGui::GetMainViewport());
			ImGui::PopStyleColor(2);
		#pragma endregion

		if (!update_game())
		{
			CloseWindow();
		}

  	rlImGuiEnd();
		EndDrawing();
	}

	
	#pragma region shutdown_cleanup
	CloseWindow();
	close_game();
	rlImGuiShutdown();
	#pragma endregion

	return 0;
}