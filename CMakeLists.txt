cmake_minimum_required(VERSION 3.27)
project(GasFlowControlMonitoringApp)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)

# Include SDL2 directories
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(imgui)
include_directories(modules)
include_directories(libs)
include_directories(libs/include)

#find_library(GAMENETWORKING_LIB NAMES GameNetworkingSockets PATHS ${PROJECT_SOURCE_DIR})
set(Boost_USE_STATIC_LIBS   ON)

#find_package(Boost COMPONENTS ASIO REQUIRED)


add_executable(GasFlowControlMonitoringApp main.cpp
        modules/PlotWindow.cpp
        modules/ControlPanel.cpp
        modules/PID.cpp
        modules/ServerModule.cpp
        modules/mainMenu.cpp
        resources/ExoFontEmbedded_utf8.cpp
)

# Set the linker flags
set(CMAKE_EXE_LINKER_FLAGS " -static")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Set the target properties
set_target_properties(GasFlowControlMonitoringApp PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(GasFlowControlMonitoringApp PROPERTIES LINK_SEARCH_END_STATIC 1)

# Set the library suffixes
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")


# Link SDL2
target_link_libraries(GasFlowControlMonitoringApp PUBLIC imgui imgui-sdl2 imgui-opengl2 ${SDL2_LIBRARIES} )

