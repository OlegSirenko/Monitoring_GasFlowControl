cmake_minimum_required(VERSION 3.27)
project(Monitor)

set(CMAKE_CXX_STANDARD 17)
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif ()

# Find SDL2
find_package(SDL2 CONFIG REQUIRED)

if(WIN32)
#    find_package(Freetype CONFIG REQUIRED)
endif ()

include_directories(${SDL2_INCLUDE_DIRS})


MESSAGE(Current dir: ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(libs)
add_subdirectory(libs/imgui)
include_directories(modules)

if (WIN32)
    add_executable(Monitor WIN32 main.cpp
            # modules
            modules/PlotWindow.cpp
            modules/ControlPanel.cpp
            modules/PID.cpp
            modules/ServerModule.cpp
            modules/mainMenu.cpp
            # libs
            libs/implot/implot.cpp
            libs/implot/implot_items.cpp
            libs/include/ImGuiNotify.hpp
#            libs/include/json.hpp
            # fonts and icons
            resources/include/ExoFontEmbedded_utf8.cpp
            resources/include/IconsFontAwesome6.h
            resources/include/icon_256.c
            resources/include/icon_256_gnome.c

    )
endif ()

if (UNIX)
add_executable(Monitor main.cpp
        # modules
        modules/PlotWindow.cpp
        modules/ControlPanel.cpp
        modules/PID.cpp
        modules/ServerModule.cpp
        modules/mainMenu.cpp
        # fonts and icons
        resources/include/ExoFontEmbedded_utf8.cpp
        resources/include/icon_256.c
        resources/include/icon_256_gnome.c
        resources/include/IconsFontAwesome6.h

        # libs
        libs/implot/implot.cpp
        libs/implot/implot_items.cpp
        libs/include/ImGuiNotify.hpp
        #        libs/include/json.hpp
#        modules/GradientDescent.cpp
#        modules/GradientDescent.h
)
endif ()

#if(WIN32)
#    set(CMAKE_EXE_LINKER_FLAGS " -static")
#    target_link_libraries(Monitor PUBLIC ws2_32 wsock32 dwmapi)
#endif()

if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    add_definitions(-D_WIN32_WINNT=0x0601)
    set_target_properties(Monitor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    target_link_libraries(Monitor PUBLIC ws2_32 wsock32 dwmapi)
endif()



target_link_libraries(Monitor PUBLIC imgui imgui-sdl2 imgui-sdlrenderer2 imgui-opengl2 ${SDL2_LIBRARIES})

if(WIN32)
#    target_link_options(Monitor PRIVATE -mwindows)
endif()


# Add the Python test script to CMake tests
enable_testing()
if(WIN32)
    add_test(NAME RunPythonClient COMMAND ${CMAKE_COMMAND} -E env cmd /c "python ${CMAKE_CURRENT_SOURCE_DIR}/Tests/Pid_regulator_test/Connection_unittest.py")
else()
    add_test(NAME RunPythonClient COMMAND ${CMAKE_COMMAND} -E env sh -c "python3 ${CMAKE_CURRENT_SOURCE_DIR}/Tests/Pid_regulator_test/Connection_unittest.py")
endif()