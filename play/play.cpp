#include "play.h"

#include <glad/glad.h>

bool
play(GLFWwindow* window, const char* profilePath, int fontSize)
{
  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  return false;
}
