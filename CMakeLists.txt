cmake_minimum_required(VERSION 3.27)
project(GasFlowControlMonitoringApp)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Find SDL2
find_package(SDL2 CONFIG REQUIRED)

if(WIN32)
#    find_package(Freetype CONFIG REQUIRED)
endif ()

# Include SDL2 directories
include_directories(${SDL2_INCLUDE_DIRS})


MESSAGE(Current dir: ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(libs)
add_subdirectory(libs/imgui)
include_directories(modules)

if (WIN32)
    add_executable(GasFlowControlMonitoringApp WIN32 main.cpp
            modules/PlotWindow.cpp
            modules/ControlPanel.cpp
            modules/PID.cpp
            modules/ServerModule.cpp
            modules/mainMenu.cpp
            resources/ExoFontEmbedded_utf8.cpp
            resources/icon_256.c
            resources/icon_256_gnome.c
            libs/implot/implot.cpp
            libs/implot/implot_items.cpp
    )
endif ()

if (UNIX)
add_executable(GasFlowControlMonitoringApp main.cpp
        modules/PlotWindow.cpp
        modules/ControlPanel.cpp
        modules/PID.cpp
        modules/ServerModule.cpp
        modules/mainMenu.cpp
        resources/ExoFontEmbedded_utf8.cpp
        resources/icon_256.c
        resources/icon_256_gnome.c
        libs/implot/implot.cpp
        libs/implot/implot_items.cpp
)
endif ()

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS " -static")
    target_link_libraries(GasFlowControlMonitoringApp PUBLIC ws2_32 wsock32 dwmapi)
endif()

target_link_libraries(GasFlowControlMonitoringApp PUBLIC imgui imgui-sdl2 imgui-sdlrenderer2 imgui-opengl2 ${SDL2_LIBRARIES})

if(WIN32)
    target_link_options(GasFlowControlMonitoringApp PRIVATE -mwindows)
endif()
