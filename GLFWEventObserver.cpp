#include "GLFWEventObserver.h"

namespace {

GLFWEventObserver*
getObserver(GLFWwindow* window)
{
  return (GLFWEventObserver*)glfwGetWindowUserPointer(window);
}

void
windowResizeCallback(GLFWwindow* window, int w, int h)
{
  getObserver(window)->framebufferResizeEvent(w, h);
}

void
framebufferResizeCallback(GLFWwindow* window, int w, int h)
{
  getObserver(window)->windowResizeEvent(w, h);
}

void
cursorPosCallback(GLFWwindow* window, double x, double y)
{
  getObserver(window)->mouseMoveEvent(x, y);
}

void
mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  if (action == GLFW_PRESS)
    getObserver(window)->mousePressEvent(button, mods);
  else if (action == GLFW_RELEASE)
    getObserver(window)->mouseReleaseEvent(button, mods);
}

void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
    getObserver(window)->keyPressEvent(key, scancode, mods);
  else if (action == GLFW_RELEASE)
    getObserver(window)->keyReleaseEvent(key, scancode, mods);
}

void
cursorEnterCallback(GLFWwindow* window, int entered)
{
  getObserver(window)->cursorEnterEvent(entered);
}

void
scrollCallback(GLFWwindow* window, double x, double y)
{
  getObserver(window)->scrollEvent(x, y);
}

} // namespace

void
GLFWEventObserver::install(GLFWwindow* window, GLFWEventObserver* observer)
{
  glfwSetWindowUserPointer(window, observer);

  glfwSetWindowSizeCallback(window, windowResizeCallback);

  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

  glfwSetCursorPosCallback(window, cursorPosCallback);

  glfwSetMouseButtonCallback(window, mouseButtonCallback);

  glfwSetKeyCallback(window, keyCallback);

  glfwSetCursorEnterCallback(window, cursorEnterCallback);

  glfwSetScrollCallback(window, scrollCallback);
}
