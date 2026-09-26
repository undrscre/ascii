#pragma once
#include "Canvas.h"
#include "../tools/ToolDefinition.h"
#include "imgui.h"
#include "imgui_internal.h"

class UserState {
public:
    UserState(Canvas& canvas) : current_canvas(canvas) {};

    bool render_canvas_grid = true;
    ImColor selected_fg_col = IM_COL32(255, 255, 255, 255);
    ImColor selected_bg_col = IM_COL32(0, 0, 0, 0);

    // todo probably expand this to UTF-8 unless c++ `char` type already does that?
    char selected_character = '#';

    Canvas& current_canvas;
    ToolType current_tool = ToolType::SELECT;

    // keyboard mode
    bool keyboard_mode = false;
    int kb_cursor_x = 0;
    int kb_cursor_y = 0;
    int kb_starting_point = 0;

    // selection
    bool is_selected  = false;
    bool has_selected = false;
    ImVec2i select_start {-1, -1};
    ImVec2i select_end   {-1, -1};
};