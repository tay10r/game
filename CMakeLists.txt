cmake_minimum_required(VERSION 3.14.7)

project(game)

find_package(glfw3 REQUIRED)

add_executable(main WIN32
  main.cpp)

target_link_libraries(main PRIVATE glfw)

install(TARGETS main
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

include(CPack)
