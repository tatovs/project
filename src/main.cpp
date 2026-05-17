#include <iostream>

#include <raylib.h>
#include <imgui.h>
#include <rlimgui/rlImGui.h>

int main()
{

	SetTraceLogLevel(LOG_ALL);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "test");

	if (!IsWindowReady())
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
		rlImGuiBegin();

		#pragma region ImGui_docking
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

		#pragma region imgui_challenge
		struct window_data
		{
			bool show_main_menu_bar = false;
			bool show_asset_browser = false;
		};

		static window_data challenge_data;

		static bool active_tool = true;
		static int radio_button = 0;
		static int int_input = 0;
		static float f = 0.0f;
		static float rgb[3] = {0.0f, 0.0f, 0.0f};
		static bool checkbox_1 = true;
		static bool checkbox_2 = true;
		
		if (challenge_data.show_main_menu_bar)
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					ImGui::MenuItem("(Testing for LLGD Challenge)", NULL, false, false),
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					ImGui::MenuItem("Undo", "Ctrl+Z");
					ImGui::MenuItem("Redo", "Ctrl+Y", false, false);
					ImGui::Separator();
					ImGui::MenuItem("Cut", "Ctrl+X");
					ImGui::MenuItem("Copy", "Ctrl+C");
					ImGui::MenuItem("Paste", "Ctrl+V");
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}

		if (challenge_data.show_asset_browser)
		{
			ImGui::Begin("Asset Browser");
			ImGui::End();
		}

		ImGui::ShowDemoWindow();

		ImGui::Begin("Task 3", &active_tool, ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();

		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::MenuItem("(demo menu)", NULL, false, false);
			ImGui::MenuItem("New");
			ImGui::MenuItem("Open", "Ctrl+O");
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::MenuItem("Save", "Ctrl+S");
			ImGui::MenuItem("Save as ..");
			ImGui::Separator();
			if (ImGui::BeginMenu("Options"))
			{
				ImGui::MenuItem("Enable", NULL, &checkbox_1);
				ImGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Borders);
				for (int i = 0; i < 10; i++)
				{
					ImGui::Text("Scrolling Text %d", i);
				}
				ImGui::EndChild();
				static float v = 0.5f;
				float v_min = 0.0f;
				float v_max = 1.0f;
				ImGui::SliderFloat("Value", &v, v_min, v_max);
				ImGui::InputFloat("Input", &v, 0.1f);
				static int n_pos = 0;
				ImGui::Combo("Combo", &n_pos, "Yes\0No\0Maybe\0\0");
				ImGui::Checkbox("Other option", &checkbox_2);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Colors"))
			{
				float sz = ImGui::GetTextLineHeight();
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
            ImGui::Dummy(ImVec2(sz, sz));
            ImGui::SameLine();
            ImGui::MenuItem(name);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Disable", false))
			{
				ImGui::EndMenu();
			}
			ImGui::MenuItem("Checked", NULL, true);
			ImGui::Separator();
			ImGui::MenuItem("Quit", "Alt+F4");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Examples"))
		{
			ImGui::MenuItem("Main menu bar", NULL, &challenge_data.show_main_menu_bar);
			ImGui::SeparatorText("Mini Apps");
			ImGui::MenuItem("Asset Browser", NULL, &challenge_data.show_asset_browser);
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();

		ImGui::Text("dear imgui says hello! (1.92.7) (19270)");
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Radio Button Testing"))
		{
			ImGui::RadioButton("Radio Button Test 0", &radio_button, 0);
			ImGui::RadioButton("Radio Button Test 1", &radio_button, 1);
			ImGui::RadioButton("Radio Button Test 2", &radio_button, 2);
		}

		ImGui::InputInt("Int Input Testing", &int_input, 1, 5, ImGuiInputTextFlags_CharsDecimal);
		ImGui::DragFloat("Drag Float Testing", &f, 0.5f, 0.0f, 100.0f);
		ImGui::VSliderFloat("Vertical Slider Testing", ImVec2(10, 50), &f, 0.0f, 100.0f);
		ImGui::SliderAngle("Angle Slider Testing", &f, 0.0f, 180.0f);
		ImGui::ColorEdit3("Color Edit3 Testing", rgb);
		if (ImGui::CollapsingHeader("Color Picker4"))
		{
			ImGui::ColorPicker4("Color Picker4 Testing", rgb);
		}
		ImGui::ProgressBar(0.5f);
		if (ImGui::TreeNode("Tree Node Testing"))
		{
			ImGui::Text("Node1");
			ImGui::Text("Node2");
			ImGui::Text("Node3");
			ImGui::Text("Node4");
			ImGui::TreePop();
		}

		ImGui::End();
		#pragma endregion

		if (x_pos >= 800)
		{
			x_pos = 0;
		}

		x_pos++;

  	rlImGuiEnd();
		EndDrawing();
	}

	#pragma region shutdown_cleanup
	  rlImGuiShutdown();
	  CloseWindow();
	#pragma endregion

	return 0;
}