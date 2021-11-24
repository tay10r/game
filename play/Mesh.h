#pragma once

#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <vector>

struct MeshVertex final
{
  glm::vec3 position;

  glm::vec3 normal;

  glm::vec2 texCoords;
};

struct Mesh final
{
  using Vertex = MeshVertex;

  glm::vec3 diffuse{ 0.8f, 0.8f, 0.8f };

  glm::vec3 emission{ 0.0f, 0.0f, 0.0f };

  std::vector<Vertex> vertices;

  const void* vertexBufferData() const noexcept { return vertices.data(); }

  GLsizeiptr vertexBufferSize() const noexcept { return vertices.size() * sizeof(Vertex); }
};

struct OpenGLMesh final
{
  glm::vec3 diffuse{ 0.8f, 0.8f, 0.8f };

  glm::vec3 emission{ 0.0f, 0.0f, 0.0f };

  OpenGLVertexBuffer<glm::vec3, glm::vec3, glm::vec2> vertexBuffer;

  OpenGLMesh(const Mesh& mesh)
    : diffuse(mesh.diffuse)
    , emission(mesh.emission)
  {
    vertexBuffer.bind();

    vertexBuffer.allocate(mesh.vertices.size(), GL_STATIC_DRAW);

    using OpenGLVertex = OpenGLVertexBuffer<glm::vec3, glm::vec3, glm::vec2>::Vertex;

    vertexBuffer.write(0, (OpenGLVertex*)&mesh.vertices[0], mesh.vertices.size());

    vertexBuffer.unbind();
  }

  void bind() { vertexBuffer.bind(); }

  void unbind() { vertexBuffer.unbind(); }
};
