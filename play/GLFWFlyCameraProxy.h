#pragma once

#include "Constants.h"
#include "FlyCamera.h"
#include "GLFWEventObserver.h"

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <optional>
#include <vector>

template<typename Float>
class GLFWFlyCameraProxy final : public GLFWEventObserver
{
public:
  using vec2 = glm::tvec2<Float>;

  using vec3 = glm::tvec3<Float>;

  GLFWFlyCameraProxy(FlyCamera<Float>* camera)
    : m_camera(camera)
  {}

  void update()
  {
    if (m_lastMousePosition) {
      if (m_movePoints.size() < 2) {
        m_movePoints.emplace_back(m_lastMousePosition.value());
      } else {
        m_movePoints[0] = m_movePoints[1];
        m_movePoints[1] = m_lastMousePosition.value();
      }
    }

    if ((m_movePoints.size() > 1) && ((glm::length(m_movePoints[1] - m_movePoints[0]) > 0.00001)) && m_dragState) {

      vec2 delta = m_movePoints[1] - m_movePoints[0];

      m_movePoints.erase(m_movePoints.begin());

      m_camera->applyMouseMotion(delta);
    }

    m_camera->applyRelativeMove(worldSpaceMoveVector());
  }

  void mousePressEvent(int button, int) override
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
      m_dragState = true;
  }

  void mouseReleaseEvent(int button, int) override
  {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
      m_dragState = false;
  }

  void mouseMoveEvent(double x, double y) { m_lastMousePosition = vec2(x, y); }

  void keyPressEvent(int key, int /* scancode */, int /* mods */) { keyEvent<true>(key); }

  void keyReleaseEvent(int key, int /* scancode */, int /* mods */) { keyEvent<false>(key); }

private:
  vec3 worldSpaceMoveVector() const
  {
    const std::optional<vec2> moveVec = moveVector();
    if (!moveVec)
      return vec3(0, 0, 0);

    return glm::normalize((m_camera->forwardVector() * moveVec->x) + (m_camera->rightVector() * moveVec->y));
  }

  std::optional<vec2> moveVector() const
  {
    int x = 0;
    int y = 0;

    if (m_moveState[0])
      x += 1;

    if (m_moveState[1])
      y -= 1;

    if (m_moveState[2])
      x -= 1;

    if (m_moveState[3])
      y += 1;

    if ((x == 0) && (y == 0))
      return std::nullopt;
    else
      return glm::normalize(vec2(x, y));
  }

  template<bool IsPress>
  void keyEvent(int key)
  {
    switch (key) {
      case GLFW_KEY_W:
      case GLFW_KEY_UP:
        m_moveState[0] = IsPress;
        break;
      case GLFW_KEY_A:
      case GLFW_KEY_LEFT:
        m_moveState[1] = IsPress;
        break;
      case GLFW_KEY_S:
      case GLFW_KEY_DOWN:
        m_moveState[2] = IsPress;
        break;
      case GLFW_KEY_D:
      case GLFW_KEY_RIGHT:
        m_moveState[3] = IsPress;
        break;
    }
  }

private:
  bool m_moveState[4]{ false, false, false, false };

  std::vector<std::chrono::high_resolution_clock::time_point> m_timePoints;

  std::optional<vec2> m_lastMousePosition;

  std::vector<vec2> m_movePoints;

  bool m_dragState = false;

  FlyCamera<Float>* m_camera;
};
