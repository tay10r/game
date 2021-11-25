#include "Scene.h"

#include "Camera.h"

bool
Scene::compileShaders(std::ostream& errStream)
{
  if (!m_atmosphere.compileShaderProgram(errStream))
    return false;

  return true;
}

void
Scene::render(const Camera<float>& camera)
{
  m_atmosphere.setRotation(camera.worldToCameraRotation());

  m_atmosphere.render();
}

void
Scene::runPhysics()
{}
