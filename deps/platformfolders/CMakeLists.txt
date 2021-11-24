cmake_minimum_required(VERSION 3.9.6)

add_library(platformfolders platform_folders.cpp)

target_include_directories(platformfolders PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
