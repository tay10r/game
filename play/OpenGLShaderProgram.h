#pragma once

#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <cassert>

class OpenGLShaderProgram final
{
public:
  static OpenGLShaderProgram null() { return OpenGLShaderProgram(Null{}); }

  static OpenGLShaderProgram makeSimpleProgram(const char* vertSource, const char* fragSource, std::ostream& errStream);

  OpenGLShaderProgram()
    : m_programID(glCreateProgram())
  {}

  OpenGLShaderProgram(const OpenGLShaderProgram&) = delete;

  OpenGLShaderProgram(OpenGLShaderProgram&& other)
    : m_boundFlag(other.m_boundFlag)
    , m_programID(other.m_programID)
  {
    other.m_boundFlag = false;
    other.m_programID = 0;
  }

  ~OpenGLShaderProgram()
  {
    if (m_programID > 0)
      glDeleteProgram(m_programID);
  }

  bool isNull() const { return !m_programID; }

  bool isLinked() const
  {
    if (isNull())
      return false;

    return getLinkStatus() != GL_FALSE;
  }

  void bind()
  {
    assert(!m_boundFlag);

    m_boundFlag = true;

    glUseProgram(m_programID);
  }

  void unbind()
  {
    assert(m_boundFlag);

    m_boundFlag = false;

    glUseProgram(0);
  }

  bool isBound() const { return m_boundFlag; }

  void setUniformValue(GLint location, const glm::vec3& value)
  {
    assert(m_boundFlag);

    assert(location >= 0);

    glUniform3f(location, value.x, value.y, value.z);
  }

  void setUniformValue(GLint location, const glm::mat3& value)
  {
    assert(m_boundFlag);

    assert(location >= 0);

    glUniformMatrix3fv(location, 1, GL_FALSE, (const float*)&value[0]);
  }

  void setUniformValue(GLint location, const glm::mat4& value)
  {
    assert(m_boundFlag);

    assert(location >= 0);

    glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)&value[0]);
  }

  GLint getUniformLocation(const char* name) const
  {
    assert(m_boundFlag);

    return glGetUniformLocation(m_programID, name);
  }

  GLuint getProgramID() { return m_programID; }

  void attach(OpenGLShader& shader) { glAttachShader(m_programID, shader.getShaderID()); }

  void detach(OpenGLShader& shader) { glDetachShader(m_programID, shader.getShaderID()); }

  void link() { glLinkProgram(m_programID); }

  GLint getIntegerValue(GLenum kind) const
  {
    GLint value = 0;

    glGetProgramiv(m_programID, kind, &value);

    return value;
  }

  GLint getLinkStatus() const { return getIntegerValue(GL_LINK_STATUS); }

  GLint getInfoLogLength() const { return getIntegerValue(GL_INFO_LOG_LENGTH); }

  std::string getInfoLog() const
  {
    const GLint logLength = getInfoLogLength();

    if (logLength < 0)
      return std::string();

    std::string infoLog(size_t(logLength), ' ');

    GLsizei actualLength = 0;

    glGetProgramInfoLog(m_programID, logLength, &actualLength, &infoLog[0]);

    if (actualLength < 0)
      return std::string();

    infoLog.resize(size_t(actualLength));

    return infoLog;
  }

private:
  class Null final
  {};

  OpenGLShaderProgram(Null)
    : m_programID(0)
  {}

private:
  bool m_boundFlag = false;

  GLuint m_programID = 0;
};
