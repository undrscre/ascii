#pragma once
#include "../core/State.h"
#include "../core/Canvas.h"

class ITool {
public:
    const char name[20] = "Unknown";

    virtual ~ITool() = default;
    virtual void RenderToolOptions(UserState& state) = 0;
    virtual void OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) = 0;
};