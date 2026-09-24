#pragma once

#include "imgui.h"
#include "../core/State.h"
#include "../core/Canvas.h"

class Viewport {
public:
    float zoom_factor = 1.0f;
    ImVec2 pan_offset = ImVec2(0.0f, 0.0f);

    // style
    ImColor grid_color_first = IM_COL32(50, 50, 50, 255);
    ImColor grid_color_secondary = IM_COL32(80, 80, 80, 255);
    
    ImVec2 char_size;
    ImVec2 canvas_pos;
    ImVec2 canvas_size;

    int kb_cursor_x, kb_cursor_y, kb_starting_point = 0;
    // TODO: render with custom font
    void RenderCanvas(UserState& state, Canvas& canvas);
    
private:
    // TODO: decouple into like. a general helper lib or WHATEVER
    bool ScreenToCell(ImVec2 mouse_pos, ImVec2 canvas_pos, ImVec2 char_size, int& out_x, int& out_y) {
        out_x = (int)((mouse_pos.x - canvas_pos.x) / char_size.x);
        out_y = (int)((mouse_pos.y - canvas_pos.y) / char_size.y);
        return true;
    };

    void DrawHoverHighlight(ImDrawList* draw_list, const Canvas& canvas, const UserState& state);
    void HandleNavigation(UserState& state);
    void DrawGrid(ImDrawList* draw_list, const Canvas& canvas);

    // todo move this somewhere
    void HandleToolInteraction(Canvas& canvas, UserState& state);
    void DrawCells(ImDrawList* draw_list, const Canvas& canvas);
    void DrawKeyboardHighlight(ImDrawList* draw_list, const Canvas& canvas, const UserState& state);
    void DrawStatusBar(ImDrawList* draw_list, const UserState& state);
    void HandleKeyboardMode(Canvas& canvas, UserState& state);
};