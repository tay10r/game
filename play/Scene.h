#pragma once

#include "Atmosphere.h"

template<typename Float>
class Camera;

class Scene final
{
public:
  bool compileShaders(std::ostream& errStream);

  void render(const Camera<float>& camera);

  void runPhysics();

private:
  Atmosphere m_atmosphere;
};
