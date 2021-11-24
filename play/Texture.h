#pragma once

#include "Scalar.h"

#include <glm/glm.hpp>

#include <memory>

#include <cmath>

template<typename Texel>
class Texture final
{
public:
  Texture() = default;

  Texture(int w, int h)
    : m_data(new Texel[w * h])
    , m_width(w)
    , m_height(h)
  {}

  constexpr bool empty() const noexcept { return size() == 0; }

  constexpr int size() const noexcept { return m_width * m_height; }

  constexpr int width() const noexcept { return m_width; }

  constexpr int height() const noexcept { return m_height; }

  Texel& operator[](int index) noexcept { return m_data.get()[index]; }

  const Texel& operator[](int index) const noexcept { return m_data.get()[index]; }

private:
  std::unique_ptr<Texel[]> m_data;
  int m_width = 0;
  int m_height = 0;
};

template<typename Texel>
Texel
texture(const Texture<Texel>& texture, int x, int y)
{
  if (texture.empty())
    return Texel{};

  x = clamp(x, 0, texture.width() - 1);
  y = clamp(y, 0, texture.height() - 1);

  return texture[(y * texture.width()) + x];
}

template<typename Texel>
Texel
texture(const Texture<Texel>& texture, const glm::vec2& uv)
{
  const float x = uv.x * texture.width();
  const float y = uv.y * texture.height();

  const int x0 = x;
  const int y0 = y;

  const int x1 = std::ceil(x);
  const int y1 = std::ceil(y);

  const float w0 = x - x0;
  const float w1 = y - y0;

  const Texel t0 = texture(texture, x0, y0);
  const Texel t1 = texture(texture, x1, y0);
  const Texel t2 = texture(texture, x0, y1);
  const Texel t3 = texture(texture, x1, y1);

  const Texel a0 = t0;
  const Texel a1 = t1 - t0;
  const Texel a2 = t2 - t0;
  const Texel a3 = t3 - t1 - t2 + t0;

  return a0 + (a1 * w0) + (a2 * w1) + (a3 * w0 * w1);
}

bool
savePNG(const Texture<float>&, const char* path);

bool
savePNG(const Texture<glm::vec3>&, const char* path);

bool
savePNG(const Texture<glm::vec4>&, const char* path);
