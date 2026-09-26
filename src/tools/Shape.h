#pragma once 
#include "ITool.h"

enum Shape {
    SQUARE,
    LINE,
    CIRCLE,
    TOTAL
};


class ShapeTool : public ITool {
public:
    enum Shape selected_shape = Shape::SQUARE;

    void RenderToolOptions(UserState& state) override;
    void OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) override {};
private:
};