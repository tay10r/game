#pragma once

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "GLFWCameraProxy.h"
#include "Scene.h"
#include "UVTimer.h"

#include "../GLFWEventObserver.h"

class Game;
class CompositeGLFWEventObserver;

class GameRenderCallback final : public UVTimerObserver
{
public:
  GameRenderCallback(Game* game)
    : m_game(game)
  {}

  void onTimeout(UVTimer*) override;

private:
  Game* m_game;
};

class GamePhysicsCallback final : public UVTimerObserver
{
public:
  GamePhysicsCallback(Game* game)
    : m_game(game)
  {}

  void onTimeout(UVTimer*) override;

private:
  Game* m_game;
};

class GameInputCallback final : public UVTimerObserver
{
public:
  GameInputCallback(Game* game)
    : m_game(game)
  {}

  void onTimeout(UVTimer*) override;

private:
  Game* m_game;
};

class GameMenuCallback final : public GLFWEventObserver
{
public:
  GameMenuCallback(Game* game)
    : m_game(game)
  {}

  void keyPressEvent(int key, int scancode, int mods) override;

private:
  Game* m_game;
};

class Game final
{
public:
  Game(uv_loop_t* loop, GLFWwindow* window, int fontSize);

  bool compileShaders(std::ostream& errStream);

  bool windowShouldClose() { return glfwWindowShouldClose(m_window); }

  void connectEventObservers(CompositeGLFWEventObserver*);

  void render();

  void runPhysics();

  void updateInput();

  void start();

  void stop();

  void startInput();

  void stopInput();

  void startRendering();

  void stopRendering();

  void startPhysics();

  void stopPhysics();

  void setMenuEnabled(bool enabled) { m_menuEnabled = enabled; }

  bool menuEnabled() const noexcept { return m_menuEnabled; }

  void toggleMenu();

private:
  void renderGame();

  void renderMenu();

private:
  int m_fontSize = 24;

  bool m_menuEnabled = false;

  int m_inputFrequency = 50;

  int m_renderFrequency = 30;

  int m_physicsFrequency = 100;

  GLFWwindow* m_window;

  GameMenuCallback m_menuCallback;

  GameInputCallback m_inputCallback;

  GameRenderCallback m_renderCallback;

  GamePhysicsCallback m_physicsCallback;

  UVTimer m_inputTimer;

  UVTimer m_renderTimer;

  UVTimer m_physicsTimer;

  Camera<float> m_camera;

  GLFWCameraProxy<float> m_cameraProxy;

  Scene m_scene;
};
