cmake_minimum_required(VERSION 3.14.7)

project(game VERSION 0.1.1 LANGUAGES CXX C)

add_subdirectory(deps)
add_subdirectory(glad)

add_executable(main WIN32
  main.cpp
  Settings.h
  Settings.cpp
  GLFWEventObserver.h
  GLFWEventObserver.cpp
  startup-menu/runStartupMenu.h
  startup-menu/runStartupMenu.cpp
  play/play.h
  play/play.cpp
  play/Game.h
  play/Game.cpp
  play/Scene.h
  play/Scene.cpp
  play/UVTimer.h
  play/UVTimer.cpp)

target_compile_features(main PRIVATE cxx_std_14)

target_compile_definitions(main
  PRIVATE
    GLFW_INCLUDE_NONE=1
    "ASSETS_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/assets\""
    "PROJECT_NAME=\"${PROJECT_NAME}\"")

target_link_libraries(main PRIVATE glfw glad uv imgui platformfolders)

install(TARGETS main
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

include(CPack)
