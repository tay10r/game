#pragma once

#include <iosfwd>

#include <glm/fwd.hpp>

class TerrainImpl;

class Terrain final
{
public:
  ~Terrain();

  bool compileShaders(std::ostream& errStream);

  void render(const glm::mat4& proj, const glm::mat4& view);

  void renderGui();

private:
  TerrainImpl& getSelf();

private:
  TerrainImpl* m_self = nullptr;
};
