#include "Brush.h"
#include "imgui.h"

void BrushTool::RenderToolOptions(UserState& state) {
    ImGui::Text("Tool options for Brush:");
    ImGui::Separator();
    ImGui::SliderInt("Brush size", &brush_size, 1, 40);
    ImGui::Separator();
    ImGui::Text("lalala");
    ImGui::Text("lalalalalala");
    ImGui::Text("lalalalalalaaawawawawa");
}

void BrushTool::OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) {
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        canvas.SetCell(cell_x, cell_y, state.selected_character, state.selected_fg_col, state.selected_bg_col);
    } else if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        canvas.SetCell(cell_x, cell_y, ' ', 0xFFFFFFFF, 0xFFFFFFFF);
    };
    return;
}