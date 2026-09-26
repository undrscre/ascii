#pragma once
#include "ITool.h"

enum BrushType {
    NORMAL,
    RECOLOR,
    REPLACE,
    RANDOM
};

class BrushTool : public ITool {
public:
    int brush_size = 1;
    BrushType selected_type = BrushType::NORMAL;

    bool rainbow_mode = false;

    void RenderToolOptions(UserState& state) override;
    void OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) override;
    void OnCanvasHover(ImDrawList* draw_list, const UserState& state, const Canvas& canvas, ImVec2 canvas_pos, ImVec2 char_size) override;
};