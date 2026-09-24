#include "MenuBar.h"
#include "../core/State.h"
#include "imgui.h"

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