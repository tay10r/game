#include "runStartupMenu.h"

#include <glad/glad.h>

bool
runStartupMenu(GLFWwindow* window)
{
  glClearColor(0, 0, 0, 1);

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  return true;
}
