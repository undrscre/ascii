#include "CharacterPalette.h"

void CharacterPalette::RenderCharPalette(UserState& state) {
    ImGui::Begin("Character Palette", nullptr);

    ImGui::SetWindowFontScale(4.0);
    ImVec2 base_char_size = ImGui::CalcTextSize("M");
    ImVec2 grid_size(
        base_char_size.x + 5,
        base_char_size.y + 5
    );

    int columns = std::max(1, (int)(ImGui::GetWindowSize().x / (grid_size.x + ImGui::GetStyle().WindowPadding.x)));
    for (int i = 32; i < 127; i++) {
        char c = static_cast<char>(i);
        bool is_selected = (c == state.selected_character);

        ImGui::PushID(static_cast<int>(i));
        if (ImGui::Selectable(&c, is_selected, 0, grid_size)) {
            state.selected_character = c;
        }

        ImGui::PopID();

        if ((i + 1) % columns != 0) {
            ImGui::SameLine();
        }
    }

    ImGui::SetWindowFontScale(1.0);
    ImGui::End();
}