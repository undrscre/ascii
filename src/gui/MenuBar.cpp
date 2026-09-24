#include "MenuBar.h"
#include "../core/State.h"
#include "imgui.h"

void MenuBar::RenderBar(UserState& state) {
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