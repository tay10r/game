#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <iostream>

#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

#include "startup-menu/runStartupMenu.h"

#ifdef _WIN32
INT WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, INT)
#else
int
main()
#endif
{
  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Failed to initialize GLFW." << std::endl;
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

  GLFWwindow* window = glfwCreateWindow(1280, 720, "", nullptr, nullptr);

  if (!window) {
    std::cerr << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

  glfwSwapInterval(1);

  runStartupMenu(window);

  glfwDestroyWindow(window);

  glfwTerminate();

  return EXIT_SUCCESS;
}
