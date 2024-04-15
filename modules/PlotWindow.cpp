//
// Created by tehnokrat on 4/10/24.
//

#include "PlotWindow.h"

void PlotWindow::Render(bool connection_emitted, long times_delta, double current_data, double pid_output) {
    if(attach_window){
        ImGui::SetNextWindowSize(ImVec2(window_width * 2 / 3, window_height)); // Set "Test Plot" size to 2/3 of SDL window width and full height
        ImGui::SetNextWindowPos(ImVec2(window_position_x + window_width * 1 / 3, window_position_y)); // Set "Test Plot" position to right of "New Window"
    }
    // Add the current time and framerate to your data
    if(connection_emitted){
        times.push_back(times_delta);
        framerates.push_back(current_data);
        pid_outs.push_back(pid_output);
    }


    if(ImGui::Begin("Plot")){
        if(ImPlot::BeginPlot("Data from Sensor") ){
            ImPlot::SetupAxes("Time, ms", "Data from sensor", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Sensor input", times.data(), framerates.data(), framerates.size());
            ImPlot::PlotLine("PID output", times.data(), pid_outs.data(), pid_outs.size());
        }
        ImPlot::EndPlot();
    }
    ImGui::Separator();
    if(ImGui::BeginChild("Data")){
        std::string output = "Framerate " + std::to_string(current_data) + " At " + std::to_string(times_delta / 1000) + " seconds from started application";
        ImGui::Text("%s", output.c_str());
    }
    ImGui::EndChild();
    ImGui::End();
}



void PlotWindow::Render(bool connection_emitted, long times_delta, double current_data) {
    if(attach_window){
        ImGui::SetNextWindowSize(ImVec2(window_width * 2 / 3, window_height)); // Set "Test Plot" size to 2/3 of SDL window width and full height
        ImGui::SetNextWindowPos(ImVec2(window_position_x + window_width * 1 / 3, window_position_y)); // Set "Test Plot" position to right of "New Window"
    }
    // Add the current time and framerate to your data
    if (connection_emitted) {
        times.push_back(times_delta);
        framerates.push_back(current_data);
    }

    if(ImGui::Begin("Plot")){
        if(ImPlot::BeginPlot("Data from Sensor") ){
            ImPlot::SetupAxes("Time, ms", "Data from sensor", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            ImPlot::PlotLine("Sensor input", times.data(), framerates.data(), framerates.size());
        }
        ImPlot::EndPlot();
    }
    ImGui::Separator();
    if(ImGui::BeginChild("Data")){
        std::string output = "Framerate " + std::to_string(current_data) + " At " + std::to_string(times_delta / 1000) + " seconds from started application";
        ImGui::Text("%s", output.c_str());
    }
    ImGui::EndChild();
    ImGui::End();
}