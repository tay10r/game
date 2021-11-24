#include "Scene.h"

bool
Scene::compileShaders(std::ostream& errStream)
{
  if (!m_atmosphere.compileShaderProgram(errStream))
    return false;

  return true;
}

void
Scene::render()
{
  m_atmosphere.render();
}

void
Scene::runPhysics()
{}
