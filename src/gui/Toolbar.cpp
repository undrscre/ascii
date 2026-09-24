#include "Toolbar.h"
#include "imgui.h"
#include "imgui_internal.h"

void Toolbar::RenderToolbar(UserState& state) {
    if (ImGui::BeginViewportSideBar("Toolbar", ImGui::GetMainViewport(), ImGuiDir_Down, 36.f, ImGuiWindowFlags_NoScrollbar)) {
        if (ImGui::RadioButton("Brush", state.current_tool == ToolType::Brush))   state.current_tool = ToolType::Brush;  ImGui::SameLine();
        if (ImGui::RadioButton("Picker", state.current_tool == ToolType::Picker)) state.current_tool = ToolType::Picker; ImGui::SameLine();

        ImVec4 fg_col = ImGui::ColorConvertU32ToFloat4(state.selected_fg_col);
        ImVec4 bg_col = ImGui::ColorConvertU32ToFloat4(state.selected_bg_col);

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();

        ImGui::Text("Foreground:"); ImGui::SameLine();
        ImGui::SetNextItemWidth(120.0f);
        if (ImGui::ColorEdit4("##foreground_color", &fg_col.x, ImGuiColorEditFlags_NoInputs)) {
            state.selected_fg_col = ImGui::ColorConvertFloat4ToU32(fg_col);
        } ImGui::SameLine();

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
        ImGui::Text("Background:"); ImGui::SameLine();
        ImGui::SetNextItemWidth(120.0f);
        if (ImGui::ColorEdit4("##background_color", &bg_col.x, ImGuiColorEditFlags_NoInputs)) {
            state.selected_bg_col = ImGui::ColorConvertFloat4ToU32(bg_col);
        }; ImGui::SameLine();

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
        ImGui::Checkbox("Grid", &state.render_canvas_grid); ImGui::SameLine();

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
        ImGui::Text("Selected character: %c", state.selected_character); ImGui::SameLine();

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical); ImGui::SameLine();
        ImGui::Text("Canvas size: %ix%i", state.current_canvas.width, state.current_canvas.height); ImGui::SameLine();

        ImGui::End();
    }
}