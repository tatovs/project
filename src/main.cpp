#include <iostream>

#include <raylib.h>
#include <imgui.h>
#include <rlimgui/rlImGui.h>

int main()
{

	SetTraceLogLevel(LOG_ALL);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "test");

	if (!IsWindowReady)
	{
		TraceLog(LOG_ERROR, "Couldn't Initialize raylib Window.");
	}

	#pragma region ImGui
		rlImGuiSetup(true);

		ImGuiIO &io = ImGui::GetIO();
		//io.ConfigFlags | ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Control
		//io.ConfigFlags | ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Control
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking.
		io.FontGlobalScale = 2.5; // Adjust Font scaling.
	#pragma endregion

	SetTargetFPS(60);
	int x_pos = 30;
	int y_pos = 30;
	int size = 100;

	while (!WindowShouldClose())
	{

		/*
			Drawing Logic
		*/
		BeginDrawing();
		ClearBackground(BLACK);

		rlImGuiBegin();

		DrawText("Testing Text Drawing", 5, 5, 20, WHITE);
		DrawRectangle(x_pos, y_pos, size, size, RED);

		DrawRectangle(75, 75, size, size, { 0, 255, 0, 255 });
		DrawRectangle(50, 50, size, size, {255, 0, 0, 255});
		
		/*
			ImGui test window 1
		*/
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		if (ImGui::Button("Test Buttton"))
		{
			std::cout << "Pressed Test Button 1" << std::endl;
		}
		ImGui::SameLine();
		ImGui::PushID(2);
		if (ImGui::Button("Test Button"))
		{
			TraceLog(LOG_DEBUG, "Pressed Test Button 2");
		}
		ImGui::PopID();
		ImGui::End();

		/*
			ImGui test window 2
		*/

		ImGui::Begin("New Testing Window");
		
		ImGui::Text("Hello again.");
		ImGui::Separator();
		ImGui::NewLine();
		static float a = 0.0;
		ImGui::SliderFloat("slider", &a, 0, 1);

		ImGui::End();

		if (x_pos >= 800)
		{
			x_pos = 0;
		}

		x_pos++;

		rlImGuiEnd();
		EndDrawing();

		
	}

	/*
		Cleanup
	*/
	rlImGuiShutdown();
	CloseWindow();

	return 0;
}