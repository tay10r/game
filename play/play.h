#pragma once

#include <GLFW/glfw3.h>

class CompositeGLFWEventObserver;

bool
play(GLFWwindow* window, CompositeGLFWEventObserver*, const char* profilePath, int fontSize);
