cmake_minimum_required(VERSION 3.9.6)

project(deps)

include(FetchContent)

if(NOT EMSCRIPTEN)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(glfw URL "https://github.com/glfw/glfw/archive/refs/tags/3.3.5.zip")
  FetchContent_MakeAvailable(glfw)
endif(NOT EMSCRIPTEN)

FetchContent_Declare(glm URL "https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip")
FetchContent_MakeAvailable(glm)

FetchContent_Declare(uv URL "https://github.com/libuv/libuv/archive/refs/tags/v1.42.0.zip")
FetchContent_MakeAvailable(uv)

FetchContent_Declare(imgui URL "https://github.com/ocornut/imgui/archive/refs/tags/v1.85.zip")
FetchContent_MakeAvailable(imgui)
FetchContent_GetProperties(imgui)

add_library(imgui
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

target_include_directories(imgui
  PUBLIC
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends")
