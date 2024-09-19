#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include "implot/implot.h"
#include <memory>
#include <cstdio>
#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <chrono>
#include <fstream>

#include "ControlPanel.h"
#include "PlotWindow.h"
#include "resources/include/ExoFontEmbedded_utf8.cpp"
#include "resources/include/IconsFontAwesome6.h"
#include "libs/include/ImGuiNotify.hpp"
#include "mainMenu.h"
#include "ServerModule.h"
#include <boost/asio.hpp>
#include <thread>
#include "imgui_internal.h"
#include "resources/include/icon_256_gnome.c"
#include "resources/include/fa-regular-400.h"
#include "resources/include/fa-solid-900.h"

void handle_events(bool&, SDL_Window*);
void update_plot_windows(const std::shared_ptr<tcp_server>& server,
                         std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>>& plotWindowsMap,
                         int window_width, int window_height, int window_position_x,
                         int window_position_y, bool attach_window);
void render_windows(const std::shared_ptr<tcp_server>& server, std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>>& plotWindowsMap,  double current_time);
std::string get_server_ip();

void embraceTheDarkness();

static void SetSDLIcon(SDL_Window* window) {
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = (my_icon.bytes_per_pixel == 3) ? 8 : 0;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = (icon.bytes_per_pixel == 3) ? 0 : 0xff000000;
#endif
    SDL_Surface* sdl_icon = SDL_CreateRGBSurfaceFrom(const_cast<void*>(static_cast<const void*>(icon.pixel_data)),
                                                 icon.width, icon.height, icon.bytes_per_pixel*8,
                                                 icon.bytes_per_pixel*icon.width, rmask, gmask, bmask, amask);
    SDL_SetWindowIcon(window, sdl_icon);

    SDL_FreeSurface(sdl_icon);
}


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
    constexpr auto window_flags = static_cast<SDL_WindowFlags>(
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Monitoring for Gas FLow control system", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);


    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }

    SetSDLIcon(window);
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

    embraceTheDarkness();


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

    constexpr float baseFontSize = 17.0f; // Default font size
    constexpr float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    ImFont* main_font = io.Fonts->AddFontFromMemoryCompressedTTF(ExoFont_compressed_data, ExoFont_compressed_size, baseFontSize);
    io.Fonts->AddFontDefault();

    static constexpr ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    ImFont* notify_font =  io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, iconFontSize, &iconsConfig, iconsRanges);

    constexpr auto clear_color = ImVec4(0.0, 0.0f, 0.0f, 1.00f);

    // Main loop
    bool done = false;
    bool connection_emitted = false;

    int window_height, window_width, window_position_x, window_position_y;

    const auto start = std::chrono::system_clock::now();

    // Init server context and thread
    auto io_context = std::make_shared<boost::asio::io_context>();
    const auto server = std::make_shared<tcp_server>(*io_context);


    const auto controlPanel = std::make_unique<ControlPanel>(window_width, window_height, window_position_x, window_position_y);

    std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>> plotWindowsMap;

    std::vector<std::string> logs = {
            "Application opened successfully",
            "Logging started...",
    };


    // Start server
    std::thread server_thread([&io_context, &logs] {
        logs.emplace_back("Trying to get the IP address...");
        logs.emplace_back("Server started at: " + get_server_ip());
        io_context->run();
        logs.emplace_back("Server closed");
    });

    while (!done){
        handle_events(done, window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        const double current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        ImGui::PushFont(main_font);

        mainMenu::Render();

        connection_emitted = server->get_connections_count() > 0;
        controlPanel->num_connections = server->get_connections_count();

        if(connection_emitted){
            update_plot_windows(server, plotWindowsMap, window_width, window_height, window_position_x, window_position_y, true);
            // Render each PlotWindow
            auto connections = server->get_connections();
            render_windows(server, plotWindowsMap, current_time);
        }
        controlPanel->Render(connection_emitted, logs);
        ImGui::PopFont();
        SDL_GetWindowSize(window, &window_width, &window_height);
        SDL_GetWindowPosition(window, &window_position_x, &window_position_y);



        // Notifications style setup
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f); // Disable round borders
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f); // Disable borders
        ImGui::PushFont(notify_font);
        ImGui::RenderNotifications();
        ImGui::PopStyleVar(2);
        ImGui::PopFont();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
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


void update_plot_windows(const std::shared_ptr<tcp_server>& server, std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>>& plotWindowsMap, int window_width, int window_height, int window_position_x, int window_position_y, bool attach_window) {
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


void render_windows(const std::shared_ptr<tcp_server>& server, std::unordered_map<tcp_connection::pointer, std::unique_ptr<PlotWindow>>& plotWindowsMap, const double current_time) {
    auto connections = server->get_connections();
    for (auto & connection : connections) {
        const auto& plotWindow = plotWindowsMap[connection];
        if(std::string data(connection->get_latest_data()); !data.empty()){
            const double current_data_from_connection = std::stod(data);
            const std::string plot_window_name = connection->get_ip() + ":" + std::to_string(connection->get_port());
            plotWindow->Render(current_time, current_data_from_connection, plot_window_name);
            connection->send_data(std::to_string(plotWindow->GetPidOutput()));
        }
    }
}

std::string get_server_ip() {
    std::string ip;
    try {
        boost::asio::io_service netService;
        boost::asio::ip::udp::resolver   resolver(netService);
        boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), "google.com", "");
        boost::asio::ip::udp::resolver::iterator endpoints = resolver.resolve(query);
        boost::asio::ip::udp::endpoint ep = *endpoints;
        boost::asio::ip::udp::socket socket(netService);
        socket.connect(ep);
        boost::asio::ip::address addr = socket.local_endpoint().address();
        ip = addr.to_string();
        ImGui::InsertNotification({ImGuiToastType::Success, 10000, "Server started at %s:12000", addr.to_string().c_str()});
    } catch (std::exception& e){
        std::cerr << "Could not deal with socket. Exception: " << e.what() << std::endl;
        ImGui::InsertNotification({ImGuiToastType::Error, 1000, "Could not deal with socket. Exception: %s",  e.what()});
        ImGui::InsertNotification({ImGuiToastType::Info, 10000, "Server Started on localhost:12000 \n You still could connect to server!"});
        ip = "127.0.0.1";
    }
    return ip+":12000";
}
