#pragma once
#include "imgui.h"
#include "../core/State.h"

class MenuBar {
public:
    void RenderBar(UserState& state) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl + N")) show_new_canvas_popup = true;
                if (ImGui::MenuItem("Open")) {}
                if (ImGui::MenuItem("Save")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Export")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {}

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("todo")) {}
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (show_new_canvas_popup) {
            ImGui::OpenPopup("Setup Canvas", ImGuiPopupFlags_AnyPopupLevel);
            show_new_canvas_popup = false;
        }

        HandleNewCanvasPopup(state);
    }
private:
    // refactor this as app grows
    bool show_new_canvas_popup = false;
    void HandleNewCanvasPopup(UserState& State);
};