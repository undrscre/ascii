#include "MenuBar.h"
#include "../core/State.h"
#include "imgui.h"
#include <cstddef>
#include <format>
#include <string>

#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "?"
#endif

void MenuBar::RenderBar(UserState& state) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) show_new_canvas_popup = true;
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

        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Reset view")) {
                
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Zoom In")) {}
            if (ImGui::MenuItem("Zoom Out")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Toggle grid", NULL, state.render_canvas_grid)) {
                state.render_canvas_grid = !state.render_canvas_grid;
            }

            ImGui::EndMenu();
        }

        std::string versionStr = std::format("Version {:.6}", GIT_COMMIT_HASH).c_str();

        float x_pos = ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(versionStr.c_str()).x;

        if (x_pos > ImGui::GetCursorPosX()) {
            ImGui::SetCursorPosX(x_pos);
        }

        ImGui::TextDisabled("%s", versionStr.c_str());

        ImGui::EndMainMenuBar();
    }

    if (show_new_canvas_popup) {
        ImGui::OpenPopup("Setup Canvas", ImGuiPopupFlags_AnyPopupLevel);
        show_new_canvas_popup = false;
    }

    HandleNewCanvasPopup(state);
}

void MenuBar::HandleNewCanvasPopup(UserState& state) {
    static int canvas_width = 80;
    static int canvas_height = 24;

    if (ImGui::BeginPopupModal("Setup Canvas", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Configure your new canvas:");

        ImGui::InputInt("Width", &canvas_width);
        ImGui::InputInt("Height", &canvas_height);

        if (canvas_width < 1) canvas_width = 1;
        if (canvas_height < 1) canvas_height = 1;

        ImGui::Separator();

        if (ImGui::Button("Confirm")) {
            Canvas new_canvas = Canvas(canvas_width, canvas_height);
            state.current_canvas = new_canvas;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}