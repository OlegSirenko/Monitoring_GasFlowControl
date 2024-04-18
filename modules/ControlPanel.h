//
// Created by OlegSirenko on 4/10/24.
//

#ifndef IMGUI_GASFLOWCONTROLL_CONTROLPANEL_H
#define IMGUI_GASFLOWCONTROLL_CONTROLPANEL_H

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include "implot.h"
#include <iostream>
#include <vector>
#include <chrono>

class ControlPanel {
public:
    ControlPanel(int& window_width, int& window_height, int& window_position_x, int& window_position_y):
                    window_width(window_width), window_height(window_height),
                    window_position_x(window_position_x), window_position_y(window_position_y){}

    void Render(bool&, bool&, std::vector<std::string>&);

private:
    int& window_width;
    int& window_height;
    int& window_position_x;
    int& window_position_y;

    std::string connection_button_label = "Connect";
    std::string autotune_button_label = "Enable";
};


#endif //IMGUI_GASFLOWCONTROLL_CONTROLPANEL_H
