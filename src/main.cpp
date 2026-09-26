#include <GL/gl.h>

#include "gui/ToolOptions.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

// my data type :)
#include "core/State.h"
#include "core/Canvas.h"

#include "gui/CharacterPalette.h"
#include "gui/Viewport.h"
#include "gui/MenuBar.h"
#include "gui/Toolbar.h"
#include "gui/ToolOptions.h"
#include "tools/ToolManager.h"

GLFWwindow* initializeWindow() {
    // Initialize GLFW window
    if (!glfwInit()) return nullptr;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ascii", nullptr, nullptr);
    if (!window) { glfwTerminate(); return nullptr; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return window;
};

// refactor maybe
void CaptureGlobalInput(UserState& state) {
    if (ImGui::IsKeyPressed(ImGuiKey_Tab, false)) {
        state.keyboard_mode = !state.keyboard_mode;
    }

    if (ImGui::GetIO().WantCaptureKeyboard || state.keyboard_mode) return;

    if (ImGui::IsKeyPressed(ImGuiKey_1, false)) state.current_tool = ToolType::SELECT;
    if (ImGui::IsKeyPressed(ImGuiKey_2, false)) state.current_tool = ToolType::BRUSH;
    if (ImGui::IsKeyPressed(ImGuiKey_3, false)) state.current_tool = ToolType::PICKER;
    if (ImGui::IsKeyPressed(ImGuiKey_4, false)) state.current_tool = ToolType::SHAPE;
}

int main() {
    GLFWwindow* window = initializeWindow();
    if (window == nullptr) { return -1; }

    Canvas canvas(80, 25);
    UserState state(canvas);

    ToolManager toolman;

    Toolbar toolbar;
    MenuBar menu_bar;
    CharacterPalette character_palette;
    Viewport viewport;
    ToolOptions tool_options;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        CaptureGlobalInput(state);
        tool_options.RenderToolOptions(state, toolman);
        menu_bar.RenderBar(state);
        character_palette.RenderCharPalette(state);
        viewport.RenderCanvas(state, state.current_canvas, toolman);
        toolbar.RenderToolbar(state, toolman);

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

