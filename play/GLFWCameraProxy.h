#pragma once

#include "Camera.h"
#include "Constants.h"

#include "../GLFWEventObserver.h"

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

template<typename Float>
class GLFWCameraProxy final : public GLFWEventObserver
{
public:
  using vec2 = glm::tvec2<Float>;

  using vec3 = glm::tvec3<Float>;

  GLFWCameraProxy(Camera<Float>* camera);

  void update();

  void mouseMoveEvent(double x, double y) override;

  void keyPressEvent(int key, int, int) override { keyEvent<true>(key); }

  void keyReleaseEvent(int key, int, int) override { keyEvent<false>(key); }

  void framebufferResizeEvent(int w, int h) override { m_camera->setAspectRatio(Float(w) / Float(h)); }

private:
  vec3 worldSpaceMoveVector() const;

  vec2 moveVector() const;

  template<bool IsPress>
  void keyEvent(int key);

private:
  bool m_moveState[4]{ false, false, false, false };

  std::vector<vec2> m_movePoints;

  bool m_dragState = false;

  Camera<Float>* m_camera;
};

template<typename Float>
GLFWCameraProxy<Float>::GLFWCameraProxy(Camera<Float>* camera)
  : m_camera(camera)
{}

template<typename Float>
void
GLFWCameraProxy<Float>::mouseMoveEvent(double x, double y)
{
  if (m_movePoints.size() < 2) {
    m_movePoints.emplace_back(vec2(x, y));
  } else {
    m_movePoints[0] = m_movePoints[1];
    m_movePoints[1] = vec2(x, y);
  }
}

template<typename Float>
void
GLFWCameraProxy<Float>::update()
{
  if ((m_movePoints.size() > 1) && ((glm::length(m_movePoints[1] - m_movePoints[0]) > 0.00001))) {

    vec2 delta = m_movePoints[1] - m_movePoints[0];

    m_movePoints.erase(m_movePoints.begin());

    m_camera->applyMouseMotion(delta);
  }

  m_camera->applyRelativeMove(worldSpaceMoveVector());
}

template<typename Float>
auto
GLFWCameraProxy<Float>::worldSpaceMoveVector() const -> vec3
{
  const vec2 moveVec = moveVector();
  if ((moveVec.x == Float(0)) && (moveVec.y == Float(0)))
    return vec3(0, 0, 0);

  return glm::normalize((m_camera->forward() * moveVec.x) + (m_camera->right() * moveVec.y));
}

template<typename Float>
auto
GLFWCameraProxy<Float>::moveVector() const -> vec2
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
    return vec2(0, 0);
  else
    return glm::normalize(vec2(x, y));
}

template<typename Float>
template<bool IsPress>
void
GLFWCameraProxy<Float>::keyEvent(int key)
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
