#include "OpenGLShaderProgram.h"

#include <iomanip>
#include <ostream>

namespace {

struct ShaderSource final
{
  const char* str;

  ShaderSource(const char* str_)
    : str(str_)
  {}
};

std::ostream&
operator<<(std::ostream& outStream, const ShaderSource& src)
{
  size_t line = 1;

  for (size_t i = 0; src.str[i] != 0; i++) {

    outStream << std::setw(4) << std::setfill('0') << line;

    outStream << " | ";

    for (size_t j = i; (src.str[j] != 0) && (src.str[j] != '\n'); j++, i++)
      outStream << src.str[j];

    outStream << std::endl;

    line++;
  }

  return outStream;
}

} // namespace

bool
OpenGLShaderProgram::compileSimpleProgram(const char* vertSource, const char* fragSource, std::ostream& errStream)
{
  OpenGLShader vertShader(GL_VERTEX_SHADER);

  vertShader << vertSource;

  vertShader.compile();

  if (!vertShader.isCompiled()) {
    errStream << ShaderSource(vertSource) << vertShader.getInfoLog();
    return false;
  }

  OpenGLShader fragShader(GL_FRAGMENT_SHADER);

  fragShader << fragSource;

  fragShader.compile();

  if (!fragShader.isCompiled()) {
    errStream << ShaderSource(fragSource) << fragShader.getInfoLog();
    return false;
  }

  attach(vertShader);
  attach(fragShader);

  link();

  detach(vertShader);
  detach(fragShader);

  if (!isLinked()) {
    errStream << getInfoLog();
    return false;
  }

  return true;
}
