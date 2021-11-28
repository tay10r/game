#pragma once

#include "Atmosphere.h"
#include "Terrain.h"

template<typename Float>
class Camera;

class Scene final
{
public:
  bool compileShaders(std::ostream& errStream);

  void render(const Camera<float>& camera);

  void renderGui();

  void runPhysics();

private:
  Atmosphere m_atmosphere;

  Terrain m_terrain;
};
