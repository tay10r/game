#pragma once

#include "OpenGLShaderProgram.h"
#include "OpenGLVertexBuffer.h"

#include <iosfwd>

class Atmosphere final
{
public:
  Atmosphere();

  bool compileShaderProgram(std::ostream& errStream);

  void render();

private:
  OpenGLShaderProgram m_program;

  OpenGLVertexBuffer<glm::vec2> m_vertexBuffer;
};
