#include "ToolOptions.h"
#include "imgui.h"

void ToolOptions::RenderToolOptions(UserState& state, ToolManager& toolman) {
    ImGui::Begin("Tool Options");
    ITool* active = toolman.GetActiveTool(state.current_tool);
    if (active != nullptr) {
        active->RenderToolOptions(state);
    } else {
        ImGui::TextDisabled("No configurable options for this tool.");
    }
    ImGui::End();
}