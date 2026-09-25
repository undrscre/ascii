#include "Select.h"
#include "imgui.h"

void SelectTool::OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) {
    if (state.keyboard_mode && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        state.kb_cursor_x = cell_x;
        state.kb_cursor_y = cell_y;
        state.kb_starting_point = cell_x;
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        
    }
}