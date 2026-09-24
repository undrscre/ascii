#include "Viewport.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <X11/X.h>
#include <cmath>

void Viewport::RenderCanvas(UserState& state, Canvas& canvas) {
    ImGui::Begin("Canvas Viewport", nullptr);

    ImVec2 base_char_size = ImGui::CalcTextSize("M");
    char_size = ImVec2(
        base_char_size.x * zoom_factor,
        base_char_size.y * zoom_factor
    );

    canvas_size = ImVec2(
        canvas.width * char_size.x, 
        canvas.height * char_size.y
    );

    ImVec2 total_avail = ImGui::GetContentRegionAvail();
    ImVec2 avail_size = ImVec2(total_avail.x, total_avail.y);

    float offset_x = (avail_size.x - canvas_size.x) * 0.5f;
    float offset_y = (avail_size.y - canvas_size.y) * 0.5f;

    ImVec2 start_cursor = ImGui::GetCursorScreenPos();
    canvas_pos = ImVec2(
        start_cursor.x + (offset_x > 0.0f ? offset_x : 0.0f) + pan_offset.x,
        start_cursor.y + (offset_y > 0.0f ? offset_y : 0.0f) + pan_offset.y
    );

    // input
    ImGui::InvisibleButton("##canvas_hitbox", avail_size, 
        ImGuiButtonFlags_MouseButtonLeft | 
        ImGuiButtonFlags_MouseButtonRight |
        ImGuiButtonFlags_MouseButtonMiddle
    );

    // ImGui::SetCursorScreenPos(canvas_pos);
    HandleNavigation(state);
    HandleToolInteraction(canvas, state);
    if (state.keyboard_mode) {
        HandleKeyboardMode(canvas, state);
    }

    // drawing
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    if (state.render_canvas_grid) DrawGrid(draw_list, canvas);
    draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), grid_color_secondary);

    DrawCells(draw_list, canvas);
    if (state.keyboard_mode) DrawKeyboardHighlight(draw_list, canvas, state);
    DrawHoverHighlight(draw_list, canvas, state);
    
    DrawStatusBar(draw_list, state);
    ImGui::End();
}

void Viewport::DrawHoverHighlight(ImDrawList* draw_list, const Canvas& canvas, const UserState& state) {
    if (ImGui::IsWindowHovered() || ImGui::IsItemActive()) {
        int hover_x, hover_y;
        ImFont* font = ImGui::GetFont();
        float font_size = ImGui::GetFontSize() * zoom_factor;

        ImVec2 mouse_pos = ImGui::GetIO().MousePos;
        ScreenToCell(mouse_pos, canvas_pos, char_size, hover_x, hover_y);

        if (hover_x >= 0 && hover_x < canvas.width && hover_y >= 0 && hover_y < canvas.height) {
            ImVec2 hover_min = ImVec2(
                canvas_pos.x + hover_x * char_size.x,
                canvas_pos.y + hover_y * char_size.y
            );
            ImVec2 hover_max = ImVec2(
                hover_min.x + char_size.x,
                hover_min.y + char_size.y
            );
            draw_list->AddRectFilled(hover_min, hover_max, IM_COL32(255, 255, 255, 30));
            draw_list->AddRect(hover_min, hover_max, IM_COL32(255, 255, 255, 120));

            ImVec2 pos(
                canvas_pos.x + hover_x * char_size.x,
                canvas_pos.y + hover_y * char_size.y
            );
            ImColor faint(
                state.selected_fg_col.Value.x,
                state.selected_fg_col.Value.y,
                state.selected_fg_col.Value.z,
                0.25f
            );
            draw_list->AddText(font, font_size, pos, faint, &state.selected_character);
        }
    }
}

void Viewport::DrawKeyboardHighlight(ImDrawList* draw_list, const Canvas& canvas, const UserState& state) {
    if (kb_cursor_x >= 0 && kb_cursor_x < canvas.width && kb_cursor_y >= 0 && kb_cursor_y < canvas.height) {
        ImVec2 hover_min = ImVec2(
            canvas_pos.x + kb_cursor_x * char_size.x,
            canvas_pos.y + kb_cursor_y * char_size.y
        );
        ImVec2 hover_max = ImVec2(
            hover_min.x + char_size.x,
            hover_min.y + char_size.y
        );

        float time = static_cast<float>(ImGui::GetTime());
        bool blink = fmodf(time, 1.0f) < 0.5f;

        if (blink) {
            draw_list->AddRectFilled(hover_min, hover_max, IM_COL32(255, 0, 255, 30));
            draw_list->AddRect(hover_min, hover_max, IM_COL32(255, 0, 255, 120));
        }
    }
}

void Viewport::HandleNavigation(UserState& state) {
    if (ImGui::IsItemActive() && ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
        ImVec2 mouse_delta = ImGui::GetIO().MouseDelta;
        pan_offset = ImVec2(
            pan_offset.x + mouse_delta.x,
            pan_offset.y + mouse_delta.y
        );
    }

    if (ImGui::IsItemHovered() && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
        float wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.0f) {
            zoom_factor += wheel * 0.1f;
            if (zoom_factor < 0.2f) zoom_factor = 0.2f;   // max zoom out
            if (zoom_factor > 5.0f) zoom_factor = 5.0f;   // max zoom in
        }
    }

    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(ImGuiKey_Tab, false)) {
        state.keyboard_mode = !state.keyboard_mode;
    }
}

void Viewport::DrawGrid(ImDrawList* draw_list, const Canvas& canvas) {
    for (int x = 0; x <= canvas.width; x++) {
        float x_pos = canvas_pos.x + x * char_size.x;
        draw_list->AddLine(
            ImVec2(x_pos, canvas_pos.y), 
            ImVec2(x_pos, canvas_pos.y + canvas_size.y), 
            x % 5 == 0 ? grid_color_secondary : grid_color_first
        );
    }

    for (int y = 0; y <= canvas.height; y++) {
        float y_pos = canvas_pos.y + y * char_size.y;
        draw_list->AddLine(
            ImVec2(canvas_pos.x, y_pos), 
            ImVec2(canvas_pos.x + canvas_size.x, y_pos), 
            y % 5 == 0 ? grid_color_secondary : grid_color_first
        );
    }
}

void Viewport::HandleToolInteraction(Canvas& canvas, UserState& state) {
    if (ImGui::IsItemDeactivated() && state.current_tool == ToolType::Picker) {
        state.current_tool = ToolType::Brush;
    }

    bool is_active = ImGui::IsItemActive();
    if (!is_active) return;

    ImVec2 mouse_pos = ImGui::GetIO().MousePos;
    int cell_x, cell_y;
    ScreenToCell(mouse_pos, canvas_pos, char_size, cell_x, cell_y);

    switch (state.current_tool) {
        case ToolType::Select:
            if (state.keyboard_mode) {
                kb_cursor_x = cell_x;
                kb_cursor_y = cell_y;
                kb_starting_point = cell_x;
            }
            break;
        case ToolType::Brush:
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                canvas.SetCell(cell_x, cell_y, state.selected_character, state.selected_fg_col, state.selected_bg_col);
            } else if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                canvas.SetCell(cell_x, cell_y, ' ', 0xFFFFFFFF, 0xFFFFFFFF);
            };
            break;

        case ToolType::Picker:
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                Cell cell = canvas.GetCell(cell_x, cell_y);
                state.selected_character = cell.glyph;
                state.selected_bg_col = cell.col_bg;
                state.selected_fg_col = cell.col_fg;
            }
            break;

        default: break;
    }
}

void Viewport::DrawCells(ImDrawList* draw_list, const Canvas& canvas) {
    ImFont* font = ImGui::GetFont();
    float font_size = ImGui::GetFontSize() * zoom_factor;
    for (int y = 0; y < canvas.height; y++) {
        for (int x = 0; x < canvas.width; x++) {
            const Cell& cell = canvas.GetCell(x, y);
            ImVec2 pos(
                canvas_pos.x + x * char_size.x,
                canvas_pos.y + y * char_size.y
            );
            char buf[2] = { cell.glyph, '\0' };
            draw_list->AddText(font, font_size, pos, cell.col_fg, buf);
        };
    };
}

void Viewport::DrawStatusBar(ImDrawList* draw_list, const UserState& state) {
    ImVec2 window_size = ImGui::GetContentRegionAvail();
    ImVec2 window_pos = ImGui::GetWindowPos();

    ImGui::SetCursorPos(ImVec2(20.0f, 35.0f));

    ImGui::BeginGroup();
    if (state.keyboard_mode) ImGui::Text("Keyboard mode activated (tab)");
    if (std::abs(zoom_factor - 1.0f) > 0.01f) {
        ImGui::Text("Zoom: %.1fx", zoom_factor);
    }

    ImGui::EndGroup();
}

void Viewport::HandleKeyboardMode(Canvas& canvas, UserState& state) {
    if (!ImGui::IsWindowFocused()) return; 

    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, true))  kb_cursor_x = ImClamp(kb_cursor_x - 1, 0, canvas.width - 1);
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, true)) kb_cursor_x = ImClamp(kb_cursor_x + 1, 0, canvas.width - 1);
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, true))    kb_cursor_y = ImClamp(kb_cursor_y - 1, 0, canvas.height - 1);
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, true))  kb_cursor_y = ImClamp(kb_cursor_y + 1, 0, canvas.height - 1);

    for (int n = 0; n < io.InputQueueCharacters.Size; n++) {
        unsigned int c = io.InputQueueCharacters[n];
        
        if (c >= 32 && c <= 126) {
            canvas.SetCell(kb_cursor_x, kb_cursor_y, static_cast<char>(c), state.selected_fg_col, state.selected_bg_col);
            kb_cursor_x++;
            if (kb_cursor_x >= canvas.width) {
                kb_cursor_x = 0;
                if (kb_cursor_y < canvas.height - 1) {
                    kb_cursor_y++;
                }
            }
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Backspace, true)) {
        if (kb_cursor_x > 0) {
            kb_cursor_x--;
        } else if (kb_cursor_y > 0) {
            kb_cursor_y--;
            kb_cursor_x = canvas.width - 1;
        }
        canvas.SetCell(kb_cursor_x, kb_cursor_y, ' ', state.selected_fg_col, state.selected_bg_col);
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Enter, true) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter, true)) {
        kb_cursor_x = kb_starting_point;
        if (kb_cursor_y < canvas.height - 1) {
            kb_cursor_y++;
        }
    }
}