#pragma once
#include "ITool.h"

class BrushTool : public ITool {
public:
    const char name[10] = "Brush";
    int brush_size = 1;

    void RenderToolOptions(UserState& state) override;
    void OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) override;
};