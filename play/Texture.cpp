#include "Texture.h"

#include "Scalar.h"

#include <stb_image_write.h>

#include <vector>

namespace sf {

bool
savePNG(const Texture<float>& texture, const char* path)
{
  const int w = texture.width();
  const int h = texture.height();

  std::vector<unsigned char> buffer(w * h);

  for (int i = 0; i < (w * h); i++) {
    buffer[i] = clamp(texture[i] * 255.0f, 0.0f, 255.0f);
  }

  return stbi_write_png(path, w, h, 1, &buffer[0], w);
}

bool
savePNG(const Texture<glm::vec3>& texture, const char* path)
{
  const int w = texture.width();
  const int h = texture.height();
  const int comp = 3;

  std::vector<unsigned char> buffer(w * h * comp);

  for (int i = 0; i < (w * h); i++) {
    buffer[(i * comp) + 0] = clamp(texture[i].r * 255.0f, 0.0f, 255.0f);
    buffer[(i * comp) + 1] = clamp(texture[i].g * 255.0f, 0.0f, 255.0f);
    buffer[(i * comp) + 2] = clamp(texture[i].b * 255.0f, 0.0f, 255.0f);
  }

  return stbi_write_png(path, w, h, comp, &buffer[0], w * comp);
}

bool
savePNG(const Texture<glm::vec4>& texture, const char* path)
{
  const int w = texture.width();
  const int h = texture.height();
  const int comp = 4;

  std::vector<unsigned char> buffer(w * h * comp);

  for (int i = 0; i < (w * h); i++) {
    buffer[(i * comp) + 0] = clamp(texture[i].r * 255.0f, 0.0f, 255.0f);
    buffer[(i * comp) + 1] = clamp(texture[i].g * 255.0f, 0.0f, 255.0f);
    buffer[(i * comp) + 2] = clamp(texture[i].b * 255.0f, 0.0f, 255.0f);
    buffer[(i * comp) + 3] = clamp(texture[i].a * 255.0f, 0.0f, 255.0f);
  }

  return stbi_write_png(path, w, h, comp, &buffer[0], w * comp);
}

} // namespace sf
