#include "Atmosphere.h"

#include <ostream>

namespace {

const char* gVertShader = R"(
#version 300 es

layout(location = 0) in vec2 position;

out highp vec2 texCoords;

void
main()
{
  texCoords = position;

  gl_Position = vec4((position.xy * 2.0) - 1.0, 0.0, 1.0);
}
)";

const char* gFragShader = R"(
#version 300 es

in highp vec2 texCoords;

out lowp vec4 outColor;

highp vec3
atmosphere(highp vec3 r,
           highp vec3 r0,
           highp vec3 pSun,
           highp float iSun,
           highp float rPlanet,
           highp float rAtmos,
           highp vec3 kRlh,
           highp float kMie,
           highp float shRlh,
           highp float shMie,
           highp float g);

void
main()
{
  highp vec3 uSunPos = vec3(1, 1, 1);

  highp vec3 vPosition = vec3((texCoords * 2.0) - 1.0, -1.0);

  highp vec3 color = atmosphere(
      normalize(vPosition),           // normalized ray direction
      vec3(0,6372e3,0),               // ray origin
      uSunPos,                        // position of the sun
      22.0,                           // intensity of the sun
      6371e3,                         // radius of the planet in meters
      6471e3,                         // radius of the atmosphere in meters
      vec3(5.5e-6, 13.0e-6, 22.4e-6), // Rayleigh scattering coefficient
      21e-6,                          // Mie scattering coefficient
      8e3,                            // Rayleigh scale height
      1.2e3,                          // Mie scale height
      0.758                           // Mie preferred scattering direction
  );

  // Apply exposure.
  color = 1.0 - exp(-1.0 * color);

  outColor = vec4(color, 1.0);
}

#define PI 3.141592

#define iSteps 16

#define jSteps 8

highp vec2
intersect_sphere(highp vec3 r0, highp vec3 rd, highp float sr)
{
  highp float a = dot(rd, rd);
  highp float b = 2.0 * dot(rd, r0);
  highp float c = dot(r0, r0) - (sr * sr);
  highp float d = (b*b) - 4.0*a*c;

  if (d < 0.0)
    return vec2(1e5,-1e5);

  return vec2(
      (-b - sqrt(d))/(2.0*a),
      (-b + sqrt(d))/(2.0*a)
  );
}

highp vec3
atmosphere(highp vec3 r,
           highp vec3 r0,
           highp vec3 pSun,
           highp float iSun,
           highp float rPlanet,
           highp float rAtmos,
           highp vec3 kRlh,
           highp float kMie,
           highp float shRlh,
           highp float shMie,
           highp float g)
{
  // Normalize the sun and view directions.
  pSun = normalize(pSun);
  r = normalize(r);

  // Calculate the step size of the primary ray.
  highp vec2 p = intersect_sphere(r0, r, rAtmos);
  if (p.x > p.y)
    return vec3(0,0,0);

  p.y = min(p.y, intersect_sphere(r0, r, rPlanet).x);

  highp float iStepSize = (p.y - p.x) / float(iSteps);

  // Initialize the primary ray time.
  highp float iTime = 0.0;

  // Initialize accumulators for Rayleigh and Mie scattering.
  highp vec3 totalRlh = vec3(0,0,0);
  highp vec3 totalMie = vec3(0,0,0);

  // Initialize optical depth accumulators for the primary ray.
  highp float iOdRlh = 0.0;
  highp float iOdMie = 0.0;

  // Calculate the Rayleigh and Mie phases.
  highp float mu = dot(r, pSun);
  highp float mumu = mu * mu;
  highp float gg = g * g;
  highp float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
  highp float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

  // Sample the primary ray.
  for (int i = 0; i < iSteps; i++) {

      // Calculate the primary ray sample position.
      highp vec3 iPos = r0 + r * (iTime + iStepSize * 0.5);

      // Calculate the height of the sample.
      highp float iHeight = length(iPos) - rPlanet;

      // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
      highp float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
      highp float odStepMie = exp(-iHeight / shMie) * iStepSize;

      // Accumulate optical depth.
      iOdRlh += odStepRlh;
      iOdMie += odStepMie;

      // Calculate the step size of the secondary ray.
      highp float jStepSize = intersect_sphere(iPos, pSun, rAtmos).y / float(jSteps);

      // Initialize the secondary ray time.
      highp float jTime = 0.0;

      // Initialize optical depth accumulators for the secondary ray.
      highp float jOdRlh = 0.0;
      highp float jOdMie = 0.0;

      // Sample the secondary ray.
      for (int j = 0; j < jSteps; j++) {

          // Calculate the secondary ray sample position.
          highp vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

          // Calculate the height of the sample.
          highp float jHeight = length(jPos) - rPlanet;

          // Accumulate the optical depth.
          jOdRlh += exp(-jHeight / shRlh) * jStepSize;
          jOdMie += exp(-jHeight / shMie) * jStepSize;

          // Increment the secondary ray time.
          jTime += jStepSize;
      }

      // Calculate attenuation.
      highp vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

      // Accumulate scattering.
      totalRlh += odStepRlh * attn;
      totalMie += odStepMie * attn;

      // Increment the primary ray time.
      iTime += iStepSize;

  }

  // Calculate and return the final color.
  return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}
)";

} // namespace

Atmosphere::Atmosphere()
{
  // clang-format off
  const float vertices[] {
    0, 0,
    0, 1,
    1, 0,
    1, 0,
    0, 1,
    1, 1
  };
  // clang-format on

  m_vertexBuffer.bind();

  m_vertexBuffer.allocate(6, GL_STATIC_DRAW);

  m_vertexBuffer.write(0, vertices, 6);

  m_vertexBuffer.unbind();
}

bool
Atmosphere::compileShaderProgram(std::ostream& errStream)
{
  OpenGLShader vertShader(GL_VERTEX_SHADER);
  vertShader << gVertShader;
  vertShader.compile();
  if (!vertShader.isCompiled()) {
    errStream << vertShader.getInfoLog();
    return false;
  }

  OpenGLShader fragShader(GL_FRAGMENT_SHADER);
  fragShader << gFragShader;
  fragShader.compile();
  if (!fragShader.isCompiled()) {
    errStream << fragShader.getInfoLog();
    return false;
  }

  m_program.attach(vertShader);
  m_program.attach(fragShader);

  m_program.link();

  m_program.detach(vertShader);
  m_program.detach(fragShader);

  if (!m_program.isLinked()) {
    errStream << m_program.getInfoLog();
    return false;
  }

  return true;
}

void
Atmosphere::render()
{
  m_vertexBuffer.bind();

  m_program.bind();

  glDrawArrays(GL_TRIANGLES, 0, 6);

  m_program.unbind();

  m_vertexBuffer.unbind();
}
