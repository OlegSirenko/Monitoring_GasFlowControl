cmake_minimum_required(VERSION 3.27)
project(ImGUI_GasFlowControll)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(imgui)

add_executable(ImGUI_GasFlowControll main.cpp)
target_link_libraries(ImGUI_GasFlowControll PUBLIC imgui imgui-sdl2 imgui-opengl2)

