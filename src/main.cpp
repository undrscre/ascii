#include <GL/gl.h>

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

GLFWwindow* initializeWindow() {
    // Initialize GLFW window
    if (!glfwInit()) return nullptr;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ascii", nullptr, nullptr);
    if (!window) { glfwTerminate(); return nullptr; }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return window;
};

int main() {
    GLFWwindow* window = initializeWindow();
    if (window == nullptr) { return -1; }

    Canvas canvas(80, 25);
    UserState state(canvas);
    canvas.SetCell(0, 0, 'A', 0xFFFFFFFF, 0x000000FF);
    canvas.SetCell(1, 1, 'B', 0xFF0000FF, 0x000000FF);
    canvas.SetCell(2, 2, 'C', 0xFF00FF00, 0x000000FF);
    canvas.SetCell(3, 3, 'D', 0xFFFF0000, 0x000000FF);

    Toolbar toolbar;
    MenuBar menu_bar;
    CharacterPalette character_palette;
    Viewport viewport;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        menu_bar.RenderBar(state);
        character_palette.RenderCharPalette(state);
        viewport.RenderCanvas(state, state.current_canvas);
        toolbar.RenderToolbar(state);

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
