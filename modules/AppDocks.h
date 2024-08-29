//
// Created by tehnokratgod on 8/29/24.
//

#ifndef APPDOCKS_H
#define APPDOCKS_H

#include "imgui.h"
#include "imgui_internal.h"

namespace AppDocks {
    bool init();
    ImGuiID getMainDock();
    ImGuiID getSecondaryDock();

class AppDocks {
    public:
        ImGuiID dock1_ = 0;
        ImGuiID dock2_ = 0;
};

} // AppDocks

#endif //APPDOCKS_H
