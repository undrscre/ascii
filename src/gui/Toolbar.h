#pragma once
#include "imgui.h"
#include "../core/State.h"
#include "imgui_internal.h"

class Toolbar {
public:
    void RenderToolbar(UserState& state) {
        if (ImGui::BeginViewportSideBar("Toolbar", ImGui::GetMainViewport(), ImGuiDir_Down, 36.f, ImGuiWindowFlags_NoScrollbar)) {
            if (ImGui::RadioButton("Brush", state.current_tool == Tools::Brush))   state.current_tool = Tools::Brush;  ImGui::SameLine();
            if (ImGui::RadioButton("Eraser", state.current_tool == Tools::Eraser)) state.current_tool = Tools::Eraser; ImGui::SameLine();
            if (ImGui::RadioButton("Picker", state.current_tool == Tools::Picker)) state.current_tool = Tools::Picker; ImGui::SameLine();

            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
            ImGui::Checkbox("Grid", &state.render_canvas_grid); ImGui::SameLine();

            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
            ImGui::Text("Selected character: %c", state.selected_character); ImGui::SameLine();

            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
            ImGui::Text("Canvas size: %ix%i", state.current_canvas.width, state.current_canvas.height);

            ImGui::End();
        }
    }
private:
};