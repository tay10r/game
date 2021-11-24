#pragma once

#include "Atmosphere.h"

class Scene final
{
public:
  bool compileShaders(std::ostream& errStream);

  void render();

  void runPhysics();

private:
  Atmosphere m_atmosphere;
};
