//
// Created by OlegSirenko on 4/10/24.
//

#ifndef IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H
#define IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H

#include "imgui.h"
#include "implot/implot.h"
#include "include/csvfile.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <PID.h>
#include <deque>
#include <iomanip>
#include <string>
#include <filesystem> // For creating directories

#include "include/ImGuiNotify.hpp"


inline std::string format_time_milliseconds(const double milliseconds) {
    const auto duration = std::chrono::milliseconds(static_cast<long long>(milliseconds));
    const auto time_point = std::chrono::system_clock::time_point(duration);
    const std::time_t time_t_format = std::chrono::system_clock::to_time_t(time_point);
    const std::tm tm_format = *std::localtime(&time_t_format);
    const auto ms = duration.count() % 1000;
    std::ostringstream oss;
    oss << std::put_time(&tm_format, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms;
    return oss.str();
}


class PlotWindow {
public:
    PlotWindow(int& window_width, int& window_height, int& window_position_x, int& window_position_y, bool& attach_window):
                    window_width(window_width), window_height(window_height),
                    window_position_x(window_position_x), window_position_y(window_position_y), attach_window(attach_window)
    {
        id = instance_count++;
        slider_kp = 0.025;
        slider_ki = 0.064;
        slider_kd = 0.28;
        slider_error = 0.5;
        sum_errors = 0;

        setvalue_ = 0;

        pid.Init(slider_kp, slider_ki, slider_kd);
        autotune_enabled = false;
        pid_enable = false;

        enable_pid_button_label = "Enable PID configuration";
        enable_autotune_button_label = "Enable autotune";
    }

    ~PlotWindow(){
        const std::string folder_path = "Plots/";
        std::filesystem::create_directory(folder_path);

        const auto t = std::time(nullptr);
        const auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        const auto datetime = oss.str();

        std::cout << datetime << std::endl;

        const std::string file_name = folder_path+"Client_" + std::to_string(instance_count) + " " + datetime + ".csv";
        csvfile output_file(file_name, ",");
        output_file << "Time" << "Input Data" << "PID Data" << endrow;
        for (size_t i = 0; i < saved_time_data_.size(); ++i) {
            output_file << format_time_milliseconds(saved_time_data_[i]) << saved_input_data_[i] << saved_pid_data_[i] << endrow;
        }
        std::cout<<"Saving all data for client_"<< instance_count << "..."<<std::endl;
        instance_count--;
    }

    void Render(double, double, std::string);
    void update_pid(double, double);
    [[nodiscard]] double GetPidOutput() const;


private:
    void widget_pid_config(double);
    bool show_plot_window = true;
    std::vector<double> times;
    std::vector<double> client_output;
    std::vector<double> pid_outs;
    int& window_width;
    int& window_position_x;
    int& window_height;
    int& window_position_y;
    bool &attach_window;

    static int instance_count;  // This is the instance_count variable
    int id;  // This is the unique ID for each instance

    float slider_kp;
    float slider_ki;
    float slider_kd;
    float slider_error;
    double setvalue_;

    PID pid;

    bool pid_enable; // flag to understand if the pid regulation is enabled; true if PID enabled -- false if disabled
    bool autotune_enabled; // flag to understand if the autotune is enabled.
    bool use_abs = true;
    std::string enable_pid_button_label;
    std::string enable_autotune_button_label;

    ImVec2 size_of_plot = {640, 640};

    std::deque<double> recent_errors;
    double sum_errors;
    const std::size_t max_errors_size = 5;
    int max_data_on_plot = 5000;

    double pid_output_{};
    double current_data_{};

    std::vector<double> saved_input_data_;
    std::vector<double> saved_pid_data_;
    std::vector<double> saved_time_data_;

};


#endif //IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H
