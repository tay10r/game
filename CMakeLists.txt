cmake_minimum_required(VERSION 3.14.7)

project(game)

add_subdirectory(deps)
add_subdirectory(glad)

add_executable(main WIN32
  main.cpp
  startup-menu/runStartupMenu.h
  startup-menu/runStartupMenu.cpp)

target_compile_definitions(main PRIVATE GLFW_INCLUDE_NONE=1)

target_link_libraries(main PRIVATE glfw glad uv imgui)

install(TARGETS main
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

include(CPack)
