//
// Created by tehnokrat on 4/10/24.
//

#include "PlotWindow.h"

#include <cmath>
#include <implot/implot_internal.h>

#include "include/ImGuiNotify.hpp"

int PlotWindow::instance_count = 0;


void PlotWindow::Render(const double time_now_ms, const double current_data, std::string window_name) {
    pid_output_ = pid.GetValue();
    current_data_ = current_data;
    times.push_back(time_now_ms);

    client_output.push_back(current_data_);
    pid_outs.push_back(pid_output_);

    if (client_output.size() > max_data_on_plot) {
        size_t excess_elements = client_output.size() - max_data_on_plot;
        client_output.erase(client_output.begin(), client_output.begin() + excess_elements);
        times.erase(times.begin(), times.begin() + excess_elements);
        pid_outs.erase(pid_outs.begin(), pid_outs.begin() + excess_elements);
    }

    window_name = "client_" + std::to_string(id);

    if(ImGui::Begin(window_name.c_str())){
        if(ImPlot::BeginPlot("Data from Sensor") ){
            ImPlot::SetupAxes("Time, ms", "Data from sensor", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit );
            ImPlot::PlotLine("Sensor input", times.data(), client_output.data(), client_output.size());
            if(pid_enable)
                ImPlot::PlotLine("PID output", times.data(), pid_outs.data(), pid_outs.size());
            ImPlot::EndPlot();
        }

    }
    ImGui::Separator();
    if(ImGui::BeginChild("Data", ImVec2(640, 20))){
        const std::string output = "Accepted data from client: " + std::to_string(current_data) + " At " + std::to_string(time_now_ms);
        ImGui::Text("%s", output.c_str());
    }
    ImGui::EndChild();

    if(ImGui::BeginChild("PID Control child widget")){
        ImGui::SeparatorText("PID control");

        if(ImGui::Button(enable_pid_button_label.c_str())){
            if(pid_enable) ImGui::InsertNotification({ImGuiToastType::Success, 5000, "PID is disabled now"});
            else ImGui::InsertNotification({ImGuiToastType::Success, 5000, "PID is enabled now"});
            ImGui::InsertNotification({ImGuiToastType::Warning, 5000, "Use PID regulation carefully!"});
            enable_pid_button_label = pid_enable ? "Enable PID configuration" : "Disable PID control" ;
            pid_enable = !pid_enable;
        }

        if(pid_enable){
            widget_pid_config(current_data);
        }
        ImGui::SeparatorText("Plot configaration");
        ImGui::SliderInt("Num values on plot", &max_data_on_plot, 100, 50000);

    }
    ImGui::EndChild();
    ImGui::End();
}

void PlotWindow::update_pid(const double set_point, const double input_data) {
    const double error = input_data - set_point;
    pid.UpdateError(error);
    recent_errors.push_back(error);
    sum_errors += error;
    if (recent_errors.size() > max_errors_size) {
        sum_errors -= recent_errors.front();
        recent_errors.pop_front();
    }

    const double average_error = sum_errors / recent_errors.size();

    if(ImGui::GetHoveredID() == ImGui::GetID("max average error")) {
        if(!std::isgreater(slider_error,average_error/2)) {
            ImGui::BeginTooltip();
            ImGui::Text("Max average error is too small!");
            ImGui::EndTooltip();
        }
    }

    if(ImGui::GetHoveredID() == ImGui::GetID("Enable autotune")) {
        if(!std::isgreater(slider_error,average_error/2)) {
            ImGui::BeginTooltip();
            ImGui::Text("Enabling autotune may broke the behaviour of your system! \nMax average error is too small!");
            ImGui::EndTooltip();
        }
    }

    if (autotune_enabled) {
        std::string output_autotune = "Started autotuning " +
                                      std::to_string(pid.Kp ) + " " +
                                      std::to_string(pid.Ki) + " " +
                                      std::to_string(pid.Kd );
        std::cout<<output_autotune<<std::endl;
        std::cout << "Before checking error: " << "average_error = " << average_error << ", slider_error = " << slider_error << ", autotune_enabled = " << autotune_enabled << std::endl;

        pid.AutoTuneController(average_error);

        if(std::isgreater(this->slider_error, std::abs(average_error))){
            autotune_enabled = false;
            output_autotune = "Autotuning ended with " +
                              std::to_string(pid.Kp ) + " " +
                              std::to_string(pid.Ki) + " " +
                              std::to_string(pid.Kd ) +
                              "\n Average Error == " + std::to_string(average_error);
            std::cout<<output_autotune<<std::endl;
            slider_kp = pid.Kp;
            slider_ki = use_abs ? fabs(pid.Ki) : pid.Ki;
            slider_kd = pid.Kd;
        }
            std::cout << "After checking error: " << "average_error = " << average_error << ", slider_error = " << slider_error << ", autotune_enabled = " << autotune_enabled << std::endl;
            std::cout<<"ENDED"<<std::endl;
        }
    else{
        pid.Kp = slider_kp;
        pid.Ki = slider_ki;
        pid.Kd = slider_kd;
    }
}

double PlotWindow::GetPidOutput() const {
    if(!pid_enable)  // if PID regulator is not enabled -> send the same data that we get
        return 0;
    return pid_output_;
}


void PlotWindow::widget_pid_config(const double current_data_) {
    ImGui::SameLine();
    enable_autotune_button_label = autotune_enabled ? "Disable autotune" : "Enable autotune";
    if(ImGui::Button(enable_autotune_button_label.c_str())){
        autotune_enabled = !autotune_enabled;
        std::cout<<"Button autotune clocked: "<<autotune_enabled<<std::endl;
    }

    ImGui::InputDouble("Set Value", &setvalue_, 1, 10, "%.6f", ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SliderFloat("Kp", &slider_kp, -1, 1);

    ImGui::SliderFloat("Ki", &slider_ki, -1, 1);
    ImGui::SameLine();
    ImGui::Checkbox("Use only positive values", &use_abs);

    ImGui::SliderFloat("Kd", &slider_kd, -1, 1);
    ImGui::SliderFloat("max average error", &slider_error, 50, 0.5, "%.5f", ImGuiSliderFlags_NoRoundToFormat);

    update_pid(setvalue_, current_data_);
}

