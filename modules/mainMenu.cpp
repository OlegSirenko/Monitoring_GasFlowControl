//
// Created by TehnokraT on 09.05.2024.
//

#include "mainMenu.h"
#include "imgui.h"
#include "implot/implot.h"
#include <iostream>
#include <vector>
#include <filesystem>
#include <SDL.h>


void embraceTheDarkness()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabSelected]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabDimmed]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabDimmedSelected]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding                     = ImVec2(8.00f, 8.00f);
    style.FramePadding                      = ImVec2(5.00f, 2.00f);
    style.CellPadding                       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
    style.IndentSpacing                     = 25;
    style.ScrollbarSize                     = 15;
    style.GrabMinSize                       = 10;
    style.WindowBorderSize                  = 1;
    style.ChildBorderSize                   = 1;
    style.PopupBorderSize                   = 1;
    style.FrameBorderSize                   = 1;
    style.TabBorderSize                     = 1;
    style.WindowRounding                    = 7;
    style.ChildRounding                     = 4;
    style.FrameRounding                     = 3;
    style.PopupRounding                     = 4;
    style.ScrollbarRounding                 = 9;
    style.GrabRounding                      = 3;
    style.LogSliderDeadzone                 = 4;
    style.TabRounding                       = 4;
}

std::vector<std::string> mainMenu::files_;
std::string mainMenu::file_to_open;
bool mainMenu::open_saved_ = false;
std::vector<double> mainMenu::time_ = {};
std::vector<double> mainMenu::input_ = {};
std::vector<double> mainMenu::pid_ = {};


static void ShowMenuFile();
static void ShowEditMenu();

std::filesystem::path getDocumentsFolder() {
#ifdef _WIN32
    const char* userProfile = getenv("USERPROFILE");
    if (userProfile) {
        return std::filesystem::path(userProfile) / "Documents";
    } else {
        throw std::runtime_error("Failed to get the USERPROFILE environment variable.");
    }
#else
    if (const char* homeDir = getenv("HOME")) {
        return std::filesystem::path(homeDir) / "Documents";
    } else {
        throw std::runtime_error("Failed to get the HOME environment variable.");
    }
#endif
}


std::vector<std::string> mainMenu::ListFilesInDirectory(const std::string& directory_path) {
    std::vector<std::string> file_list;
    if(const std::filesystem::path absolute_path = std::filesystem::absolute(directory_path); !exists(absolute_path)) {
        create_directory(absolute_path);
        std::cout<<"Created directory "<<absolute_path.string()<<std::endl;
    }

    for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
        if (entry.is_regular_file()) {
            file_list.push_back(entry.path().string());
        }
    }
    return file_list;
}


void mainMenu::Render() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ShowEditMenu();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


// Note that shortcuts are currently provided for display only
// (future version will add explicit flags to BeginMenu() to request processing shortcuts)
void mainMenu::ShowMenuFile()
{
    if(ImGui::BeginMenu("Open recent", true)) {
        files_ = ListFilesInDirectory(getDocumentsFolder().string() + "/Monitor_Saved_Plots/");
        for (const auto& file : files_) {
            if(ImGui::MenuItem(file.c_str())) {
                // clear vectors before adding new data from other file
                time_.erase(time_.begin(), time_.end());
                input_.erase(input_.begin(), input_.end());
                pid_.erase(pid_.begin(), pid_.end());

                std::filesystem::path file_path(file);
                std::filesystem::path absolute_path = std::filesystem::absolute(file_path);
                std::string absolute_path_str = absolute_path.string();

                std::cout << "Opening file: " << absolute_path_str << std::endl;

                csvfile::read(absolute_path_str, time_, input_, pid_);

                std::cout << "Number of time data points: " << time_.size() << std::endl;

                file_to_open = absolute_path_str;
                open_saved_ = true;
            }
        }
        ImGui::EndMenu();
    }
    // if (ImGui::MenuItem("Quit", "Alt+F4")) {
    //
    // }
}



void mainMenu::ShowEditMenu() {
    ImGui::MenuItem("Change style", nullptr, false, false);
    static bool light = false;
    static bool dark = true;

    if(ImGui::MenuItem("Light", "", &light)){
        dark = false;
        light = true;
        ImGui::StyleColorsLight();
    }

    if(ImGui::MenuItem("Dark", "", &dark)){
        light = false;
        dark = true;
        embraceTheDarkness();
    }
    ImGui::Separator();
}


void mainMenu::ShowChosenPlot() {
    const std::string file = file_to_open;
    if (file.empty()) {
        return;
    }

    if(open_saved_) {
        ImGui::Begin(file.c_str(), &open_saved_);

        if(ImPlot::BeginPlot("Saved data")) {
            ImPlot::PlotLine("Input Data", time_.data(), input_.data(), time_.size());
            ImPlot::PlotLine("PID Data", time_.data(), pid_.data(), time_.size());
            ImPlot::EndPlot();
        }
        ImGui::End();
    }

}




