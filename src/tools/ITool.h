#pragma once
#include "../core/State.h"
#include "../core/Canvas.h"

#include "imgui.h"

class ITool {
public:
    virtual ~ITool() = default;
    virtual void RenderToolOptions(UserState& state) {
        ImGui::TextDisabled("No configurable options for this tool.");
    };
    virtual void OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) = 0;
    virtual void OnCanvasHover(ImDrawList* draw_list, const UserState& state, const Canvas& canvas, ImVec2 canvas_pos, ImVec2 char_size) {
        int hover_x, hover_y;
        ImVec2 mouse_pos = ImGui::GetIO().MousePos;
        ScreenToCell(mouse_pos, canvas_pos, char_size, hover_x, hover_y);

        if (hover_x >= 0 && hover_x < canvas.width && hover_y >= 0 && hover_y < canvas.height) {
            ImVec2 hover_min = ImVec2(
                canvas_pos.x + hover_x * char_size.x,
                canvas_pos.y + hover_y * char_size.y
            );
            ImVec2 hover_max = ImVec2(
                hover_min.x + char_size.x,
                hover_min.y + char_size.y
            );
            draw_list->AddRectFilled(hover_min, hover_max, IM_COL32(255, 255, 255, 30));
            draw_list->AddRect(hover_min, hover_max, IM_COL32(255, 255, 255, 120));
        }
    };

    bool ScreenToCell(ImVec2 mouse_pos, ImVec2 canvas_pos, ImVec2 char_size, int& out_x, int& out_y) {
        out_x = (int)((mouse_pos.x - canvas_pos.x) / char_size.x);
        out_y = (int)((mouse_pos.y - canvas_pos.y) / char_size.y);
        return true;
    };    
};