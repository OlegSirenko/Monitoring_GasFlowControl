cmake_minimum_required(VERSION 3.27)
project(ImGUI_GasFlowControll)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(imgui)

add_executable(ImGUI_GasFlowControll main.cpp
        implot_internal.h)

set(CMAKE_EXE_LINKER_FLAGS " -static")
target_link_libraries(ImGUI_GasFlowControll PUBLIC imgui imgui-win32 imgui-dx11 d3dcompiler dwmapi)
target_link_options(ImGUI_GasFlowControll PRIVATE -mwindows)

