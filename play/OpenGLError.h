#pragma once

#include <glad/glad.h>

#include <cstdio>
#include <cstdlib>

namespace sf {

class OpenGLError final
{
public:
  static void assertNone(const char* sourceFilePath, int line)
  {
#ifndef NDEBUG
    const GLenum error = glGetError();
    if (error == GL_NO_ERROR)
      return;
    fprintf(stderr, "%s:%d: %s\n", sourceFilePath, line, toString(error));
    exit(EXIT_FAILURE);
#else
    (void)sourceFilePath;
    (void)line;
#endif
  }

  static char const* toString(GLenum error) noexcept
  {
    switch (error) {
      case GL_NO_ERROR:
        return "GL_NO_ERROR";
      case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
      case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
      case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
#if 0
      case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
      case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
      case GL_TABLE_TOO_LARGE:
        return "GL_TABLE_TOO_LARGE";
#endif
      case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
      default:
        return "<unknown-opengl-error>";
    }
  }
};

} // namespace sf
