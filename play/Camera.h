#pragma once

#include "Constants.h"

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <optional>
#include <vector>

template<typename Float>
class Camera final
{
public:
  using mat3 = glm::tmat3x3<Float>;

  using mat4 = glm::tmat4x4<Float>;

  using vec2 = glm::tvec2<Float>;

  using vec3 = glm::tvec3<Float>;

  using vec4 = glm::tvec4<Float>;

  Float verticalFov() const noexcept { return m_verticalFov; }

  Float aspectRatio() const noexcept { return m_aspect; }

  Float near() const noexcept { return m_near; }

  Float far() const noexcept { return m_far; }

  void setAspectRatio(Float aspect) { m_aspect = aspect; }

  void setVerticalFov(Float fov) { m_verticalFov = fov; }

  void setNearPlane(Float near) { m_near = near; }

  void setFarPlane(Float far) { m_far = far; }

  mat4 perspective() const noexcept { return glm::perspective(m_verticalFov, m_aspect, m_near, m_far); }

  vec3 position() const { return m_position; }

  vec3 up() const;

  vec3 forward() const;

  vec3 right() const;

  mat4 worldToCameraMatrix() const;

  mat3 worldToCameraRotation() const;

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

  Float m_aspect = Float(1);

  Float m_verticalFov = glm::radians(Float(45));

  Float m_near = Float(0.1);

  Float m_far = Float(1000);
};

template<typename Float>
glm::tvec3<Float>
Camera<Float>::forward() const
{
  return sphericalToXYZ(m_pitch, m_yaw);
}

template<typename Float>
glm::tvec3<Float>
Camera<Float>::up() const
{
  return sphericalToXYZ(m_pitch - (Pi<Float>::value() / 2), m_yaw);
}

template<typename Float>
glm::tvec3<Float>
Camera<Float>::right() const
{
  return glm::normalize(glm::cross(forward(), up()));
}

template<typename Float>
glm::tmat4x4<Float>
Camera<Float>::worldToCameraMatrix() const
{
  return glm::lookAt(m_position, m_position + forward(), vec3(0, 1, 0));
}

template<typename Float>
glm::tmat3x3<Float>
Camera<Float>::worldToCameraRotation() const
{
  return glm::mat3(right(), up(), forward());
}

template<typename Float>
void
Camera<Float>::applyMouseMotion(vec2 motion)
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
Camera<Float>::applyRelativeMove(vec3 relativeMove)
{
  m_position += relativeMove;
}
