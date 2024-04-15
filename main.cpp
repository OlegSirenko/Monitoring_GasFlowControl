#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include "implot.h"
#include <cstdio>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <vector>
#include <chrono>
#include "ControllPanel.h"
#include "PlotWindow.h"
#include "PID.h"
#include <cmath>
#include <deque>


// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Monitoring for Gas FLow control system", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;



    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();

    //IM_ASSERT(font != nullptr);
    io.Fonts->AddFontFromFileTTF("../resources/BruceForeverRegular-X3jd2.ttf", 15.0f);

    bool show_demo_window = false;
    bool show_another_window = false;
    bool show_plot_window = true;


    ImVec4 clear_color = ImVec4(0.0, 0.0f, 0.0f, 1.00f);

    // Main loop
    bool done = false;
    bool connection_emitted = false;
    int window_height, window_width, window_position_x, window_position_y;
    bool attach_window= false;
    auto start = std::chrono::system_clock::now();

    double setpoint = 0.0;
    double kp = 0.3;
    double ki = 0.005;
    double kd = 0.1;


    ControllPanel controllPanel(window_width, window_height, window_position_x, window_position_y);
    PlotWindow plotWindow(window_width, window_height, window_position_x, window_position_y, attach_window);
    PID pid;
    pid.Init(kp, ki, kd);

    std::deque<double> recent_errors;
    double sum_errors = 0.0;
    const std::size_t max_errors_size = 100;


    long x_sin = 0;


    while (!done){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        auto now = std::chrono::system_clock::now();  // Calculate the time elapsed since the start of the application in seconds
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        auto current_time = elapsed.count();

        double current_data = sin(x_sin);
        x_sin = current_time;

        double error = setpoint - current_data;
        pid.UpdateError(error);

        recent_errors.push_back(error);
        sum_errors += error;
        if (recent_errors.size() > max_errors_size) {
            sum_errors -= recent_errors.front();
            recent_errors.pop_front();
        }

        double average_error = sum_errors / recent_errors.size();
        if (abs(average_error) > 0.5) {
            pid.AutoTuneController(setpoint - pid.GetSteerValue());

        }

        SDL_GetWindowSize(window, &window_width, &window_height);
        SDL_GetWindowPosition(window, &window_position_x, &window_position_y);

        controllPanel.Render(connection_emitted);
        plotWindow.Render(connection_emitted, current_time, current_data, pid.GetSteerValue()); //  with PID Output data

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        SDL_GL_SwapWindow(window);

    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


