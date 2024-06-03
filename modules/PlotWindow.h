//
// Created by OlegSirenko on 4/10/24.
//

#ifndef IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H
#define IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H

#include "imgui.h"
#include "implot.h"
#include <iostream>
#include <vector>
#include <chrono>



class PlotWindow {
public:
    PlotWindow(int& window_width, int& window_height, int& window_position_x, int& window_position_y, bool& attach_window):
                    window_width(window_width), window_height(window_height),
                    window_position_x(window_position_x), window_position_y(window_position_y), attach_window(attach_window){
        id = instance_count++;
        window_name = "Plot" + std::to_string(id);
    }

    void Render(bool, long, double, double);
    void Render(bool, long, double);
    ~PlotWindow(){
        instance_count--;
    }

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
    std::string window_name;
};


#endif //IMGUI_GASFLOWCONTROLL_PLOTWINDOW_H
