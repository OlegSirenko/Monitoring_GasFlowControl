//
// Created by OlegSirenko on 4/10/24.
//

#ifndef IMGUI_GASFLOWCONTROLL_CONTROLLPANEL_H
#define IMGUI_GASFLOWCONTROLL_CONTROLLPANEL_H

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include "implot.h"
#include <iostream>
#include <vector>
#include <chrono>

class ControllPanel {
public:
    ControllPanel(int& window_width, int& window_height, int& window_position_x, int& window_position_y):
                    window_width(window_width), window_height(window_height),
                    window_position_x(window_position_x), window_position_y(window_position_y){}

    void Render(bool&);

private:
    int& window_width;
    int& window_height;
    int& window_position_x;
    int& window_position_y;

    std::string connection_button_label = "Connect";

    std::vector<std::string> logs = {
            "Application opened successfully",
            "Logging started...",
    };
};


#endif //IMGUI_GASFLOWCONTROLL_CONTROLLPANEL_H
