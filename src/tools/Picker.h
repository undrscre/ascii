#pragma once
#include "ITool.h"
#include "ToolDefinition.h"
#include "imgui.h"

class PickerTool : public ITool {
public:
    // void RenderToolOptions(UserState& state);
    void OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) override {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            Cell cell = canvas.GetCell(cell_x, cell_y);
            state.selected_character = cell.glyph;
            state.selected_bg_col = cell.col_bg;
            state.selected_fg_col = cell.col_fg;
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            state.current_tool = ToolType::BRUSH;
        }
    };
};