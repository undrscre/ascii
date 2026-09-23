#pragma once

#include "imgui.h"
#include "Canvas.h"
#include <tuple>

class CanvasViewport {
public:
    bool render_grid = true;
    ImColor grid_color = IM_COL32(50, 50, 50, 255);
 
    // TODO: render with custom font
    void RenderCanvas(Canvas& canvas) {
        ImGui::Begin("Canvas Viewport", nullptr);

        ImVec2 char_size   = ImGui::CalcTextSize("M");
        ImVec2 canvas_size = ImVec2(
            canvas.width * char_size.x, 
            canvas.height * char_size.y
        );

        ImVec2 avail_size = ImGui::GetContentRegionAvail();
        float offset_x = (avail_size.x - canvas_size.x) * 0.5f;
        float offset_y = (avail_size.y - canvas_size.y) * 0.5f;

        ImVec2 start_cursor = ImGui::GetCursorScreenPos();
        ImVec2 canvas_pos = ImVec2(
            start_cursor.x + (offset_x > 0.0f ? offset_x : 0.0f),
            start_cursor.y + (offset_y > 0.0f ? offset_y : 0.0f)
        );

        ImGui::SetCursorScreenPos(canvas_pos);

        // inputs
        ImGui::InvisibleButton("##canvas_hitbox", canvas_size, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

        bool is_active = ImGui::IsItemActive();
        bool is_hovered = ImGui::IsItemHovered();

        if (is_active && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            ImVec2 mouse_pos = ImGui::GetIO().MousePos;
            auto [cell_x, cell_y] = MouseToCell(mouse_pos, canvas_pos, char_size);
            canvas.SetCell(cell_x, cell_y, '#', 0xFFFFFFFF, 0xFFFFFFFF);
        }

        if (is_active && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
            ImVec2 mouse_pos = ImGui::GetIO().MousePos;
            auto [cell_x, cell_y] = MouseToCell(mouse_pos, canvas_pos, char_size);
            canvas.SetCell(cell_x, cell_y, ' ', 0xFFFFFFFF, 0xFFFFFFFF);
        }

        // drawing
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        if (render_grid) DrawGrid(draw_list, canvas, canvas_pos, char_size, canvas_size);
        if (is_hovered) {
            ImVec2 mouse_pos = ImGui::GetIO().MousePos;
            auto [hover_x, hover_y] = MouseToCell(mouse_pos, canvas_pos, char_size);
            if (hover_x >= 0 && hover_x <= canvas_size.x && hover_y >= 0 && hover_y <= canvas_size.y) {
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

        for (int y = 0; y < canvas.height; y++) {
            for (int x = 0; x < canvas.width; x++) {
                const Cell& cell = canvas.GetCell(x, y);
                ImVec2 pos(
                    canvas_pos.x + x * char_size.x,
                    canvas_pos.y + y * char_size.y
                );
                char buf[2] = { cell.glyph, '\0' };
                draw_list->AddText(pos, cell.col_fg, buf);
            };
        };
        ImGui::End();
    };

private:

    void DrawGrid(ImDrawList* draw_list, const Canvas canvas, ImVec2 canvas_pos, ImVec2 char_size, ImVec2 canvas_size) {
        for (int x = 0; x <= canvas.width; x++) {
            float x_pos = canvas_pos.x + x * char_size.x;
            draw_list->AddLine(
                ImVec2(x_pos, canvas_pos.y), 
                ImVec2(x_pos, canvas_pos.y + canvas_size.y), 
                grid_color
            );
        }

        for (int y = 0; y <= canvas.height; y++) {
            float y_pos = canvas_pos.y + y * char_size.y;
            draw_list->AddLine(
                ImVec2(canvas_pos.x, y_pos), 
                ImVec2(canvas_pos.x + canvas_size.x, y_pos), 
                grid_color
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