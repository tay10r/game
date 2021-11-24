#pragma once

#include <imgui_impl_glfw.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <vector>

class GLFWEventObserver
{
public:
  static void install(GLFWwindow* window, GLFWEventObserver* observer);

  virtual ~GLFWEventObserver() = default;

  virtual void framebufferResizeEvent(int /* width */, int /* height */) {}

  virtual void windowResizeEvent(int /* width */, int /* height */) {}

  virtual void mouseMoveEvent(double /* x */, double /* y */) {}

  virtual void mousePressEvent(int /* button */, int /* mods */) {}

  virtual void mouseReleaseEvent(int /* button */, int /* mods */) {}

  virtual void keyPressEvent(int /* key */, int /* scancode */, int /* mods */) {}

  virtual void keyReleaseEvent(int /* key */, int /* scancode */, int /* mods */) {}

  virtual void cursorEnterEvent(int /* entered */) {}

  virtual void scrollEvent(double /* x */, double /* y */) {}

  virtual void focusEvent(int /* focused */) {}
};

class CompositeGLFWEventObserver final : public GLFWEventObserver
{
public:
  void addEventObserver(GLFWEventObserver* observer) { m_observers.emplace_back(observer); }

  void framebufferResizeEvent(int width, int height) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->framebufferResizeEvent(width, height);
  }

  void windowResizeEvent(int width, int height) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->windowResizeEvent(width, height);
  }

  void mouseMoveEvent(double x, double y) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->mouseMoveEvent(x, y);
  }

  void mousePressEvent(int button, int mods) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->mousePressEvent(button, mods);
  }

  void mouseReleaseEvent(int button, int mods) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->mouseReleaseEvent(button, mods);
  }

  void keyPressEvent(int key, int scancode, int mods) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->keyPressEvent(key, scancode, mods);
  }

  void keyReleaseEvent(int key, int scancode, int mods) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->keyReleaseEvent(key, scancode, mods);
  }

  void cursorEnterEvent(int entered) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->cursorEnterEvent(entered);
  }

  void scrollEvent(double x, double y) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->scrollEvent(x, y);
  }

  void focusEvent(int focused) override
  {
    for (GLFWEventObserver* observer : m_observers)
      observer->focusEvent(focused);
  }

private:
  std::vector<GLFWEventObserver*> m_observers;
};

class ImGuiProxy final : public GLFWEventObserver
{
public:
  ImGuiProxy(GLFWwindow* window)
    : m_window(window)
  {}

  void mousePressEvent(int button, int mods) override
  {
    ImGui_ImplGlfw_MouseButtonCallback(m_window, button, GLFW_PRESS, mods);
  }

  void mouseReleaseEvent(int button, int mods) override
  {
    ImGui_ImplGlfw_MouseButtonCallback(m_window, button, GLFW_RELEASE, mods);
  }

  void keyPressEvent(int key, int scancode, int mods) override
  {
    ImGui_ImplGlfw_KeyCallback(m_window, key, scancode, GLFW_PRESS, mods);
  }

  void keyReleaseEvent(int key, int scancode, int mods) override
  {
    ImGui_ImplGlfw_KeyCallback(m_window, key, scancode, GLFW_RELEASE, mods);
  }

  void cursorEnterEvent(int entered) override { ImGui_ImplGlfw_CursorEnterCallback(m_window, entered); }

  void scrollEvent(double x, double y) override { ImGui_ImplGlfw_ScrollCallback(m_window, x, y); }

  void focusEvent(int focused) override { ImGui_ImplGlfw_WindowFocusCallback(m_window, focused); }

private:
  GLFWwindow* m_window;
};

class WindowViewportResizer final : public GLFWEventObserver
{
public:
  static GLFWEventObserver* get()
  {
    static WindowViewportResizer resizer;
    return &resizer;
  }

  void framebufferResizeEvent(int w, int h) override { glViewport(0, 0, w, h); }
};
