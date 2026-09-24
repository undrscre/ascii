#pragma once
#include "../core/State.h"

class MenuBar {
public:
    void RenderBar(UserState& state);
private:
    // refactor this as app grows
    bool show_new_canvas_popup = false;
    void HandleNewCanvasPopup(UserState& State);
};