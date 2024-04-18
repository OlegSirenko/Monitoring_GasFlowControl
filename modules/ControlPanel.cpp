//
// Created by tehnokrat on 4/10/24.
//

#include "ControlPanel.h"


void ControlPanel::Render(bool& connection_emitted, bool& autotune_enable, std::vector<std::string>& logs) {
//    ImGui::SetNextWindowSize(ImVec2(window_width * 1 / 3, window_height)); // Set "New Window" size to 1/3 of SDL window width and full height
//    ImGui::SetNextWindowPos(ImVec2(window_position_x, window_position_y)); // Set "New Window" position to top left corner

    if (ImGui::Begin("Control panel")) // begin window
    {
        if (ImGui::Button(connection_button_label.c_str())) // Buttons return true when clicked.
        {
            connection_button_label = (connection_button_label == "Connect") ? "Disconnect" : "Connect";
            logs.emplace_back(!connection_emitted ? "Connection emitted..." : "Connection lost");
            connection_emitted = !connection_emitted;
        }
        if(ImGui::Button(autotune_button_label.c_str())){
            autotune_button_label = (autotune_button_label == "Enable") ? "Disable" : "Enable";
            autotune_enable = !autotune_enable;
        }
        ImGui::Separator();
        if(ImGui::BeginChild("Logs")){
            for (const std::string& log : logs){
                ImGui::TextUnformatted(log.c_str());
            }
            // Auto scroll to the bottom when a new log is added
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::EndChild(); // end child window
        }
    }
    ImGui::End(); // Control Window
}