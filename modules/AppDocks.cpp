//
// Created by tehnokratgod on 8/29/24.
//

#include "AppDocks.h"

#include <iostream>
#include <ostream>


namespace AppDocks {
    static AppDocks appDocks;
    bool init() {
        ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();
        // Now we'll need to create our dock node:
        ImGuiID id = ImGui::GetID("MainWindowGroup"); // The string chosen here is arbitrary (it just gives us something to work with)
        ImGui::DockBuilderRemoveNode(id);             // Clear any preexisting layouts associated with the ID we just chose
        ImGui::DockBuilderAddNode(id);                // Create a new dock node to use

        // 4. Set the dock node's size and position:
        ImVec2 size = ImGui::GetMainViewport()->Size; // A decently large dock node size (600x300px) so everything displays clearly

        // Calculate the position of the dock node
        //
        // `DockBuilderSetNodePos()` will position the top-left corner of the node to the coordinate given.
        // This means that the node itself won't actually be in the center of the window; its top-left corner will.
        //
        // To fix this, we'll need to subtract half the node size from both the X and Y dimensions to move it left and up.
        // This new coordinate will be the position of the node's top-left corner that will center the node in the window.
        ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };

        // Set the size and position:
        ImGui::DockBuilderSetNodeSize(id, size);
        ImGui::DockBuilderSetNodePos(id, nodePos);

        // 5. Split the dock node to create spaces to put our windows in:

        // Split the dock node in the left direction to create our first docking space. This will be on the left side of the node.
        // (The 0.5f means that the new space will take up 50% of its parent - the dock node.)
        ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.32f, nullptr, &id);
        // +-----------+
        // |           |
        // |     1     |
        // |           |
        // +-----------+

        // Split the same dock node in the right direction to create our second docking space.
        // At this point, the dock node has two spaces, one on the left and one on the right.
        ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.5f, nullptr, &id);
        // +-----+-----+
        // |     |     |
        // |  1  |  2  |
        // |     |     |
        // +-----+-----+
        //    split ->
        std::cout<<"dock1 "<<dock1<<std::endl;
        std::cout<<"dock2 "<<dock2<<std::endl;
        appDocks.dock1_ = dock1;
        appDocks.dock2_ = dock2;
        return true;
    }

    ImGuiID getMainDock() {
        return appDocks.dock1_;
    }

    ImGuiID getSecondaryDock() {
        return appDocks.dock2_;
    }
} // AppDocks