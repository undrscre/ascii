#pragma once
#include "Canvas.h"
#include "../tools/ToolDefinition.h"
#include "imgui.h"

class UserState {
public:
    UserState(Canvas& canvas) : current_canvas(canvas) {};

    bool render_canvas_grid = true;
    ImColor selected_fg_col = IM_COL32(255, 255, 255, 255);
    ImColor selected_bg_col = IM_COL32(0, 0, 0, 0);

    // todo probably expand this to UTF-8 unless c++ `char` type already does that?
    char selected_character = '#';

    Canvas& current_canvas;
    ToolType current_tool = ToolType::Brush;

    // keyboard mode
    bool keyboard_mode = false;
};