cmake_minimum_required(VERSION 3.10)
project(ascii VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Libraries
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_library(imgui STATIC
    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui_tables.cpp

    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC 
    lib/imgui
    lib/imgui/backends
)

add_subdirectory(lib/glfw)

# Project files
add_executable(${PROJECT_NAME} 
    src/main.cpp
    
    src/gui/MenuBar.cpp
    src/gui/Viewport.cpp
    src/gui/Toolbar.cpp
    src/gui/CharacterPalette.cpp
    src/gui/ToolOptions.cpp

    src/tools/Brush.cpp
    src/tools/Select.cpp
    src/tools/Shape.cpp
)

# target_sources(..)

# Linkage
# target_link_libraries(imgui PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw
    imgui
    OpenGL::GL
)

target_include_directories(${PROJECT_NAME} PUBLIC lib/glfw/include)
target_include_directories(${PROJECT_NAME} PUBLIC lib/)

# thanks https://stackoverflow.com/questions/51727566/
execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")