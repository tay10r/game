#pragma once

#include "Constants.h"

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <optional>
#include <vector>

template<typename Float>
class FlyCamera final
{
public:
  using mat3 = glm::tmat3x3<Float>;

  using mat4 = glm::tmat4x4<Float>;

  using vec2 = glm::tvec2<Float>;

  using vec3 = glm::tvec3<Float>;

  using vec4 = glm::tvec4<Float>;

  vec3 position() const { return m_position; }

  vec3 upVector() const;

  vec3 forwardVector() const;

  vec3 rightVector() const;

  mat4 worldToCameraMatrix() const;

  mat4 cameraToWorldMatrix() const;

  mat3 cameraToWorldRotation() const;

  void applyMouseMotion(vec2 motion);

  void applyRelativeMove(vec3 relativeMove);

private:
  static vec3 sphericalToXYZ(Float pitch, Float yaw)
  {
    const Float x = std::cos(yaw) * std::sin(pitch);
    const Float y = std::sin(yaw) * std::sin(pitch);
    const Float z = std::cos(pitch);
    return vec3(y, z, -x);
  }

private:
  Float m_moveSpeed = 1;

  vec2 m_rotationSpeed = vec2(0.01 / Pi<Float>::value(), 0.01 / Pi<Float>::value());

  Float m_pitch = Pi<Float>::value() / 2;

  Float m_yaw = 0;

  vec3 m_position = vec3(0, 0, 0);
};

template<typename Float>
glm::tvec3<Float>
FlyCamera<Float>::forwardVector() const
{
  return sphericalToXYZ(m_pitch, m_yaw);
}

template<typename Float>
glm::tvec3<Float>
FlyCamera<Float>::upVector() const
{
  return sphericalToXYZ(m_pitch - (Pi<Float>::value() / 2), m_yaw);
}

template<typename Float>
glm::tvec3<Float>
FlyCamera<Float>::rightVector() const
{
  return glm::normalize(glm::cross(forwardVector(), upVector()));
}

template<typename Float>
glm::tmat4x4<Float>
FlyCamera<Float>::worldToCameraMatrix() const
{
  return glm::lookAt(m_position, m_position + forwardVector(), vec3(0, 1, 0));
}

template<typename Float>
void
FlyCamera<Float>::applyMouseMotion(vec2 motion)
{
  const Float pitchDelta = motion.y * m_rotationSpeed.y;

  const Float yawDelta = motion.x * m_rotationSpeed.x;

  m_pitch += pitchDelta;

  m_yaw += yawDelta;

  const Float epsilon = 0.000001;

  m_pitch = std::max(std::min(m_pitch, Pi<Float>::value() - epsilon), epsilon);
}

template<typename Float>
void
FlyCamera<Float>::applyRelativeMove(vec3 relativeMove)
{
  m_position += relativeMove;
}
