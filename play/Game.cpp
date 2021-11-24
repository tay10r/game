#include "Game.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Game::Game(uv_loop_t* loop, GLFWwindow* window, int fontSize)
  : m_fontSize(fontSize)
  , m_window(window)
  , m_menuCallback(this)
  , m_inputCallback(this)
  , m_renderCallback(this)
  , m_physicsCallback(this)
  , m_inputTimer(loop, &m_inputCallback)
  , m_renderTimer(loop, &m_renderCallback)
  , m_physicsTimer(loop, &m_physicsCallback)
{}

bool
Game::compileShaders(std::ostream& errStream)
{
  return m_scene.compileShaders(errStream);
}

void
Game::connectEventObservers(CompositeGLFWEventObserver* compositeEventObserver)
{
  compositeEventObserver->addEventObserver(&m_menuCallback);
}

void
Game::toggleMenu()
{
  m_menuEnabled = !m_menuEnabled;
}

void
Game::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderGame();

  if (menuEnabled())
    renderMenu();

  glfwSwapBuffers(m_window);
}

void
Game::renderGame()
{
  m_scene.render();
}

void
Game::renderMenu()
{
  int w = 0;
  int h = 0;
  glfwGetFramebufferSize(m_window, &w, &h);

  ImGui_ImplOpenGL3_NewFrame();

  ImGui_ImplGlfw_NewFrame();

  ImGui::NewFrame();

  ImGui::SetNextWindowPos(ImVec2(w / 4, h / 4));

  ImGui::SetNextWindowSize(ImVec2(w / 2, h / 2));

  const ImGuiWindowFlags windowFlags =
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

  ImGui::Begin("Menu", nullptr, windowFlags);

  if (ImGui::Button("Exit"))
    stop();

  ImGui::End();

  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void
Game::runPhysics()
{
  m_scene.runPhysics();
}

void
Game::startRendering()
{
  const int renderDelay = 1000 / m_renderFrequency;

  m_renderTimer.start(renderDelay, renderDelay);
}

void
Game::stopRendering()
{
  m_renderTimer.stop();
}

void
Game::startPhysics()
{
  const int physicsDelay = 1000 / m_physicsFrequency;

  m_physicsTimer.start(physicsDelay, physicsDelay);
}

void
Game::stopPhysics()
{
  m_physicsTimer.stop();
}

void
Game::startInput()
{
  const int inputDelay = 1000 / m_inputFrequency;

  m_inputTimer.start(inputDelay, inputDelay);
}

void
Game::stopInput()
{
  m_inputTimer.stop();
}

void
Game::start()
{
  startRendering();

  startPhysics();

  startInput();
}

void
Game::stop()
{
  stopInput();

  stopPhysics();

  stopRendering();
}

void
GameRenderCallback::onTimeout(UVTimer*)
{
  m_game->render();
}

void
GamePhysicsCallback::onTimeout(UVTimer*)
{
  m_game->runPhysics();
}

void
GameMenuCallback::keyPressEvent(int key, int scancode, int mods)
{
  if (key != GLFW_KEY_ESCAPE)
    return;

  if (m_game->menuEnabled())
    m_game->startPhysics();
  else
    m_game->stopPhysics();

  m_game->toggleMenu();
}

void
GameInputCallback::onTimeout(UVTimer*)
{
  glfwPollEvents();

  if (m_game->windowShouldClose())
    m_game->stop();
}
