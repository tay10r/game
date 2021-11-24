#pragma once

#include <GLFW/glfw3.h>

#include <string>

/// @return Either an empty string if the user wants to exit the program or the path of the profile to start running.
std::string
runStartupMenu(GLFWwindow* window, int fontSize);
