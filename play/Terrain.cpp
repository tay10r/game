#include "Terrain.h"

#include "OpenGLShaderProgram.h"
#include "OpenGLTexture2D.h"
#include "OpenGLVertexBuffer.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace {

const char* gVertShader = R"(
#version 300 es

layout(location = 0) in highp vec2 position;

uniform mat4 mvp;

uniform sampler2D height;

void
main()
{
  vec2 ndc = (position * 2.0) - 1.0;

  gl_Position = mvp * vec4(ndc.x, texture(height, position).r, ndc.y, 1.0);
}
)";

const char* gFragShader = R"(
#version 300 es

out lowp vec4 outColor;

void
main()
{
  outColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

} // namespace

class TerrainImpl final
{
  friend Terrain;

  OpenGLShaderProgram program;

  OpenGLVertexBuffer<glm::vec2> vertexBuffer;

  OpenGLTexture2D heightMap;

  glm::vec3 scale = glm::vec3(10, 1, 10);

  int terrainWidth = 0;

  int terrainHeight = 0;

  void generateTerrain(int w, int h)
  {
    terrainWidth = w;
    terrainHeight = h;
    generateVertexBuffer();
    generateHeightMap();
  }

  void generateVertexBuffer()
  {
    const int w = terrainWidth;
    const int h = terrainHeight;

    // clang-format off
    const glm::vec2 vertices[6] {
      glm::vec2(0, 0),
      glm::vec2(1, 0),
      glm::vec2(0, 1),
      glm::vec2(0, 1),
      glm::vec2(1, 0),
      glm::vec2(1, 1),
    };
    // clang-format on

    std::vector<glm::vec2> buffer(w * h * 6);

    const glm::vec2 quadSize(1.0f / w, 1.0f / h);

    for (int y = 0; y < h; y++) {

      for (int x = 0; x < w; x++) {

        const int i = ((y * w) + x) * 6;

        for (int j = 0; j < 6; j++)
          buffer[i + j] = (vertices[j] + glm::vec2(x, y)) * quadSize;
      }
    }

    vertexBuffer.bind();
    vertexBuffer.allocate(w * h * 6, GL_STATIC_DRAW);
    vertexBuffer.write(0, buffer.data(), w * h * 6);
    vertexBuffer.unbind();
  }

  void generateHeightMap()
  {
    std::vector<float> buffer(terrainWidth * terrainHeight, -1.0f);

    heightMap.bind();
    heightMap.resize(terrainWidth, terrainHeight, GL_R32F, GL_RED, GL_FLOAT);
    heightMap.write(0, 0, 0, terrainWidth, terrainHeight, buffer.data());
    heightMap.unbind();
  }
};

Terrain::~Terrain()
{
  delete m_self;
}

bool
Terrain::compileShaders(std::ostream& errStream)
{
  if (!getSelf().program.compileSimpleProgram(gVertShader, gFragShader, errStream))
    return false;

  return true;
}

void
Terrain::render(const glm::mat4& proj, const glm::mat4& view)
{
  if (!m_self)
    return;

  const glm::mat4 model = glm::scale(glm::mat4(1.0f), m_self->scale);

  m_self->program.bind();
  m_self->program.setUniformValue("mvp", proj * view * model);
  m_self->vertexBuffer.bind();
  m_self->heightMap.bind();

  glDrawArrays(GL_TRIANGLES, 0, m_self->terrainWidth * m_self->terrainHeight * 6);

  m_self->heightMap.unbind();
  m_self->vertexBuffer.unbind();
  m_self->program.unbind();
}

void
Terrain::renderGui()
{
  ImGui::Begin("Terrain Options");

  ImGui::DragFloat3("Scale", &m_self->scale[0]);

  ImGui::End();
}

TerrainImpl&
Terrain::getSelf()
{
  if (!m_self) {

    m_self = new TerrainImpl();

    m_self->generateTerrain(512, 512);
  }

  return *m_self;
}
