cmake_minimum_required(VERSION 3.5)

project(imgui)

add_library(imgui)

target_sources(imgui PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui_demo.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui_draw.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui_tables.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui_widgets.cpp"
)

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking")