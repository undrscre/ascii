#pragma once

#include "imgui.h"
#include "../core/State.h"
#include "../core/Canvas.h"
#include <tuple>

// probably offload this to a .cpp file
class CanvasViewport {
public:
    float zoom_factor = 1.0f;
    ImVec2 pan_offset = ImVec2(0.0f, 0.0f);

    ImColor grid_color_first = IM_COL32(50, 50, 50, 255);
    ImColor grid_color_secondary = IM_COL32(80, 80, 80, 255);
    
    ImVec2 char_size;
    ImVec2 canvas_pos;
    ImVec2 canvas_size;

    // TODO: render with custom font
    void RenderCanvas(UserState& state, Canvas& canvas) {
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

        ImVec2 avail_size = ImGui::GetContentRegionAvail();
        float offset_x = (avail_size.x - canvas_size.x) * 0.5f;
        float offset_y = (avail_size.y - canvas_size.y) * 0.5f;

        ImVec2 start_cursor = ImGui::GetCursorScreenPos();
        canvas_pos = ImVec2(
            start_cursor.x + (offset_x > 0.0f ? offset_x : 0.0f) + pan_offset.x,
            start_cursor.y + (offset_y > 0.0f ? offset_y : 0.0f) + pan_offset.y
        );

        ImGui::InvisibleButton("##canvas_hitbox", avail_size, 
            ImGuiButtonFlags_MouseButtonLeft | 
            ImGuiButtonFlags_MouseButtonRight |
            ImGuiButtonFlags_MouseButtonMiddle
        );

        ImGui::SetCursorScreenPos(canvas_pos);
        HandleInput(state, canvas);

        // drawing
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        if (ImGui::IsWindowHovered()) {
            ImVec2 mouse_pos = ImGui::GetIO().MousePos;
            auto [hover_x, hover_y] = MouseToCell(mouse_pos, canvas_pos, char_size);
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
            }
        }
        if (state.render_canvas_grid) DrawGrid(draw_list, canvas);
        for (int y = 0; y < canvas.height; y++) {
            for (int x = 0; x < canvas.width; x++) {
                const Cell& cell = canvas.GetCell(x, y);
                ImVec2 pos(
                    canvas_pos.x + x * char_size.x,
                    canvas_pos.y + y * char_size.y
                );
                char buf[2] = { cell.glyph, '\0' };
                draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize() * zoom_factor, pos, cell.col_fg, buf);
            };
        };
        ImGui::End();
    };
    
    void HandleInput(UserState& state, Canvas& canvas) {
        bool is_active = ImGui::IsItemActive();
        bool is_hovered = ImGui::IsItemHovered();

        if (is_active && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            ImVec2 mouse_pos = ImGui::GetIO().MousePos;
            auto [cell_x, cell_y] = MouseToCell(mouse_pos, canvas_pos, char_size);
            canvas.SetCell(cell_x, cell_y, state.selected_character , 0xFFFFFFFF, 0xFFFFFFFF);
        }

        if (is_active && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
            ImVec2 mouse_pos = ImGui::GetIO().MousePos;
            auto [cell_x, cell_y] = MouseToCell(mouse_pos, canvas_pos, char_size);
            canvas.SetCell(cell_x, cell_y, ' ', 0xFFFFFFFF, 0xFFFFFFFF);
        }

        if (is_active && ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
            ImVec2 mouse_delta = ImGui::GetIO().MouseDelta;
            pan_offset = ImVec2(
                pan_offset.x + mouse_delta.x,
                pan_offset.y + mouse_delta.y
            );
        }
        
        if (is_hovered && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
            float wheel = ImGui::GetIO().MouseWheel;
            if (wheel != 0.0f) {
                zoom_factor += wheel * 0.1f;
                if (zoom_factor < 0.2f) zoom_factor = 0.2f;   // max zoom out
                if (zoom_factor > 5.0f) zoom_factor = 5.0f;   // max zoom in
            }
        }
    }
private:

    void DrawGrid(ImDrawList* draw_list, const Canvas canvas) {
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

    // TODO: decouple into like. a general helper lib or WHATEVER
    std::tuple<int, int> MouseToCell(ImVec2 mouse_pos, ImVec2 canvas_pos, ImVec2 char_size) {
        int cell_x = (int)((mouse_pos.x - canvas_pos.x) / char_size.x);
        int cell_y = (int)((mouse_pos.y - canvas_pos.y) / char_size.y);
        return {cell_x, cell_y};
    };
};