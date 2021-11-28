cmake_minimum_required(VERSION 3.9.6)

add_library(stb
  stb_image.h
  stb_image.c)

target_include_directories(stb PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
