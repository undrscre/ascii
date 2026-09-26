#include "Brush.h"
#include "imgui.h"

#include <random>

char GetRandomASCII() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    std::uniform_int_distribution<int> distrib(32, 126);
    
    return static_cast<char>(distrib(gen));
}


void BrushTool::RenderToolOptions(UserState& state) {
    ImGui::Text("Tool options for Brush:");
    ImGui::Separator();
    ImGui::SliderInt("Brush size", &brush_size, 1, 40);
    ImGui::Separator();
    if (ImGui::RadioButton("Normal mode", selected_type == BrushType::NORMAL)) {
        selected_type = BrushType::NORMAL;
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("This does what you think it does.");

    if (ImGui::RadioButton("Recolor mode", selected_type == BrushType::RECOLOR)) {
        selected_type = BrushType::RECOLOR;
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Recolors the character within the radius.");

    if (ImGui::RadioButton("Replace mode", selected_type == BrushType::REPLACE)) {
        selected_type = BrushType::REPLACE;
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Replaces character within the radius.");

    if (ImGui::RadioButton("Random mode", selected_type == BrushType::RANDOM)) {
        selected_type = BrushType::RANDOM;
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("Draws random ASCII characters to the canvas.");

    ImGui::Separator();
    ImGui::Checkbox("Rainbow mode", &rainbow_mode);
}

void BrushTool::OnCanvasInteract(Canvas& canvas, UserState& state, int cell_x, int cell_y) {
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        int radius = brush_size - 1;

        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                int target_x = cell_x + dx;
                int target_y = cell_y + dy;

                ImColor final_col = state.selected_fg_col;
                if (rainbow_mode) {
                    float hue = fmodf(0.2f + (target_x + target_y) * 0.05f, 1.0f);

                    float r, g, b;
                    ImGui::ColorConvertHSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
                    final_col = ImColor(r, g, b, 1.0f);
                }

                Cell previous = canvas.GetCell(target_x, target_y);
                switch (selected_type) {
                    case BrushType::NORMAL:
                        canvas.SetCell(target_x, target_y, state.selected_character, final_col, state.selected_bg_col);
                        break;
                    case BrushType::RECOLOR:
                        canvas.SetCell(target_x, target_y, previous.glyph, final_col, state.selected_bg_col);
                        break;
                    case BrushType::REPLACE:
                        canvas.SetCell(target_x, target_y, state.selected_character, previous.col_fg, previous.col_bg);
                        break;
                    case BrushType::RANDOM:
                        canvas.SetCell(target_x, target_y, GetRandomASCII(), final_col, state.selected_bg_col);
                        break;
                    default: break;
                }
            }
        }

    } else if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        int radius = brush_size - 1;

        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                int target_x = cell_x + dx;
                int target_y = cell_y + dy;

                canvas.SetCell(target_x, target_y, ' ', 0xFFFFFFFF, 0xFFFFFFFF);
            }
        }
    };
    return;
}

void BrushTool::OnCanvasHover(ImDrawList* draw_list, const UserState& state, const Canvas& canvas, ImVec2 canvas_pos, ImVec2 char_size) {
    int hover_x, hover_y;
    ImVec2 mouse_pos = ImGui::GetIO().MousePos;
    ScreenToCell(mouse_pos, canvas_pos, char_size, hover_x, hover_y);

    int radius = brush_size - 1;
    if (hover_x >= 0 && hover_x < canvas.width && hover_y >= 0 && hover_y < canvas.height) {
        ImVec2 hover_min = ImVec2(
            canvas_pos.x + (hover_x - radius) * char_size.x,
            canvas_pos.y + (hover_y - radius) * char_size.y
        );
        ImVec2 hover_max = ImVec2(
            canvas_pos.x + (hover_x + radius + 1) * char_size.x,
            canvas_pos.y + (hover_y + radius + 1) * char_size.y
        );
        draw_list->AddRectFilled(hover_min, hover_max, IM_COL32(255, 255, 255, 30));
        draw_list->AddRect(hover_min, hover_max, IM_COL32(255, 255, 255, 120));
    }
};

