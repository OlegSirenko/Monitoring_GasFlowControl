cmake_minimum_required(VERSION 3.27)
project(ImGUI_GasFlowControll)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)

# Include SDL2 directories
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(imgui)

add_executable(ImGUI_GasFlowControll main.cpp)

# Set the linker flags
set(CMAKE_EXE_LINKER_FLAGS " -static")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Set the target properties
set_target_properties(ImGUI_GasFlowControll PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(ImGUI_GasFlowControll PROPERTIES LINK_SEARCH_END_STATIC 1)

# Set the library suffixes
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

# Link SDL2
target_link_libraries(ImGUI_GasFlowControll PUBLIC imgui imgui-sdl2 imgui-opengl2 ${SDL2_LIBRARIES} )


include(appimage.cmake)

# Create the AppImage
make_appimage(
        EXE "cmake-build-release/ImGUI_GasFlowControll"  # Path to your built executable
        NAME "ImGUI_GasFlowControll"  # Name of your application
        ICON "./resources/GasFlowControlIcon.svg"  # Path to your application icon (optional)
        OUTPUT_NAME "ImGUI_GasFlowControll.AppImage"  # Output AppImage filename
        DIR_ICON "./resources/GasFlowControlIcon_png.png"
        ASSETS "LICENSE"  # List of extra files to include in the AppDir (optional)
)

# Print a success message
message("AppImage created successfully!")
