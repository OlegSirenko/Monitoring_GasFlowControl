//
// Created by OlegSirenko on 4/10/24.
//

#ifndef IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H
#define IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H

#include "imgui.h"
#include "implot/implot.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <PID.h>
#include <deque>
#include <string>


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
        slider_error = 0.0004;
        sum_errors = 0;

        setvalue_ = 0;

        pid.Init(slider_kp, slider_ki, slider_kd);
        autotune_enabled = false;
        pid_enable = false;

        enable_pid_button_label = "Enable PID configuration";
        enable_autotune_button_label = "Enable autotune";
    }

    ~PlotWindow(){
        instance_count--;
    }

    void Render(long, double, std::string);
    void update_pid(double, double);
    [[nodiscard]] double GetPidOutput() const;


private:
    bool show_plot_window = true;
    std::vector<double> times;
    std::vector<double> framerates;
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
    std::string enable_pid_button_label;
    std::string enable_autotune_button_label;

    ImVec2 size_of_plot = {640, 640};

    std::deque<double> recent_errors;
    double sum_errors;
    const std::size_t max_errors_size = 100;

    double pid_output_;
    double current_data_;

};


#endif //IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H
