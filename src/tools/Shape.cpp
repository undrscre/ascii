#include "Shape.h"
#include "imgui.h"

const char* shape_names[] = {"Square", "Line", "Circle"};

void ShapeTool::RenderToolOptions(UserState& state) {
    ImGui::Text("Tool options for Shape");
    ImGui::Separator();

    if (ImGui::BeginCombo("Select Shape", shape_names[selected_shape])) {
        for (int i = 0; i < Shape::TOTAL; i++) {
            const bool is_selected = (selected_shape == i);
            if (ImGui::Selectable(shape_names[i], is_selected)) {
                selected_shape = (Shape)i;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
}