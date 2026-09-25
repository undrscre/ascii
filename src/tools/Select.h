#pragma once
#include "ITool.h"

class SelectTool : public ITool {
public:
    // void RenderToolOptions(UserState& state);
    void OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) override;
};