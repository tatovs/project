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

	#pragma region global imgui setup
	rlImGuiSetup(true);

	ImGuiIO &io = ImGui::GetIO();
	//io.ConfigFlags | ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Control
	//io.ConfigFlags | ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Control
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking.
	io.FontGlobalScale = 1; // Adjust Font scaling.
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

		#pragma region ImGui
		  rlImGuiBegin();
			ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
			ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, ImGui::GetMainViewport());
			ImGui::PopStyleColor(2);
		#pragma endregion
		DrawText("Testing Text Drawing", 5, 5, 20, WHITE);
		DrawRectangle(x_pos, y_pos, size, size, RED);

		DrawRectangle(75, 75, size, size, { 0, 255, 0, 255 });
		DrawRectangle(50, 50, size, size, {255, 0, 0, 255});
		
		#pragma region imgui window 1
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
		#pragma endregion
		
		#pragma region imgui window 2
		ImGui::Begin("New Testing Window");
		
		ImGui::Text("Hello again.");
		ImGui::Separator();
		ImGui::NewLine();
		static float a = 0.0;
		ImGui::SliderFloat("slider", &a, 0, 1);

		ImGui::End();
		#pragma endregion

		if (x_pos >= 800)
		{
			x_pos = 0;
		}

		x_pos++;

		#pragma region ImGui
  		rlImGuiEnd();
		#pragma endregion

		EndDrawing();
	}

	#pragma region shutdown_cleanup
	  rlImGuiShutdown();
	  CloseWindow();
	#pragma endregion

	return 0;
}