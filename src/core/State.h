#pragma once
#include "Canvas.h"
#include <cstdint>

enum Tools {
    Brush,
    Eraser,
    Picker,
};

class UserState {
public:
    UserState(Canvas& canvas) : current_canvas(canvas) {};

    bool render_canvas_grid = true;
    uint32_t selected_fg_col = 0xFFFFFFFF;
    uint32_t selected_bg_col = 0x00000000;
    // todo probably expand this to UTF-8 unless c++ `char` type already does that?
    char selected_character = '#'; 

    Canvas& current_canvas;
    Tools current_tool = Tools::Brush;
};