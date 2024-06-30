#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include "implot/implot.h"
#include <memory>
#include <cstdio>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <vector>
#include <chrono>
#include "ControlPanel.h"
#include "PlotWindow.h"
#include <deque>
#include "resources/ExoFontEmbedded_utf8.cpp"
#include "mainMenu.h"
#include "ServerModule.h"
#include <boost/asio.hpp>
#include <thread>

void handle_events(bool&, SDL_Window*);
void update_plot_windows(std::shared_ptr<tcp_server>& server,
                         std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>>& plotWindowsMap,
                         int window_width, int window_height, int window_position_x,
                         int window_position_y, bool attach_window);


void render_windows(std::shared_ptr<tcp_server>& server, std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>>& plotWindowsMap,  long current_time);

void embraceTheDarkness();


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
    ImGui::StyleColorsLight();

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

    io.Fonts->AddFontFromMemoryCompressedTTF(ExoFont_compressed_data, ExoFont_compressed_size, 17);

    bool show_demo_window = false;
    bool show_another_window = false;
    bool show_plot_window = true;


    ImVec4 clear_color = ImVec4(0.0, 0.0f, 0.0f, 1.00f);

    // Main loop
    bool done = false;
    bool connection_emitted = false;

    int window_height, window_width, window_position_x, window_position_y;

    auto start = std::chrono::system_clock::now();

    double setpoint = 0.0;

    // Init server context and thread
    auto io_context = std::make_shared<boost::asio::io_context>();
    auto server = std::make_shared<tcp_server>(*io_context);



    std::unique_ptr<ControlPanel> controlPanel = std::make_unique<ControlPanel>(window_width, window_height, window_position_x, window_position_y);

    std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>> plotWindowsMap;

    std::vector<std::string> logs = {
            "Application opened successfully",
            "Logging started...",
    };


    // Start server
    std::thread server_thread([&io_context, &logs] {
//        std::cout<<"Waiting for data... "<<std::endl;
        logs.emplace_back("Server started");
        io_context->run();
        logs.emplace_back("Server closed");
    });


    while (!done){
        handle_events(done, window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        //ImGui::ShowDemoWindow();
        auto now = std::chrono::system_clock::now();  // Calculate the time elapsed since the start of the application in seconds
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
        auto current_time = elapsed.count();

        mainMenu::Render();

        connection_emitted = server->get_connections_count() > 0;

        if(connection_emitted){
            update_plot_windows(server, plotWindowsMap, window_width, window_height, window_position_x, window_position_y, true);
            controlPanel->num_connections = server->get_connections_count();
            // Render each PlotWindow
            auto connections = server->get_connections();

            render_windows(server, plotWindowsMap, current_time);
        }
        controlPanel->Render(connection_emitted, logs);

        SDL_GetWindowSize(window, &window_width, &window_height);
        SDL_GetWindowPosition(window, &window_position_x, &window_position_y);


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
    io_context->stop();
    server_thread.join();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}




void handle_events(bool& done, SDL_Window* window) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            done = true;
    }
}


void update_plot_windows(std::shared_ptr<tcp_server>& server, std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>>& plotWindowsMap, int window_width, int window_height, int window_position_x, int window_position_y, bool attach_window) {
    auto connections = server->get_connections();
    // Remove any PlotWindows that don't have an associated connection
    for (auto it = plotWindowsMap.begin(); it != plotWindowsMap.end(); ) {
        if (std::find(connections.begin(), connections.end(), it->first) == connections.end()) {
            it = plotWindowsMap.erase(it);
        } else {
            ++it;
        }
    }

    // Add a PlotWindow for any new connections
    for (const auto& connection : connections) {
        if (plotWindowsMap.find(connection) == plotWindowsMap.end()) {
            plotWindowsMap[connection] = std::make_unique<PlotWindow>(window_width, window_height, window_position_x, window_position_y, attach_window);
        }
    }
}


void render_windows(std::shared_ptr<tcp_server>& server, std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>>& plotWindowsMap,  long current_time) {
    auto connections = server->get_connections();
    for (auto & connection : connections) {
        auto& plotWindow = plotWindowsMap[connection];
        std::string data(connection->get_latest_data());
        if(!data.empty()){
            double current_data_from_connection = std::stod(data);
            std::string plot_window_name = connection->get_ip() + ":" + std::to_string(connection->get_port());
            plotWindow->Render(current_time, current_data_from_connection, plot_window_name);
        }
    }
}


