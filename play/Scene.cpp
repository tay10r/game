#include "Scene.h"

#include "Camera.h"

bool
Scene::compileShaders(std::ostream& errStream)
{
  if (!m_atmosphere.compileShaderProgram(errStream))
    return false;

  if (!m_terrain.compileShaders(errStream))
    return false;

  return true;
}

void
Scene::render(const Camera<float>& camera)
{
  m_atmosphere.setRotation(camera.worldToCameraRotation());

  m_atmosphere.render();

  m_terrain.render(camera.perspective(), camera.worldToCameraMatrix());
}

void
Scene::renderGui()
{
  m_terrain.renderGui();
}

void
Scene::runPhysics()
{}
