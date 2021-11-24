#include "play.h"

#include "Game.h"

#include <glad/glad.h>

#include <uv.h>

bool
play(GLFWwindow* window, CompositeGLFWEventObserver* compositeEventObserver, const char* profilePath, int fontSize)
{
  uv_loop_t loop;

  uv_loop_init(&loop);

  Game game(&loop, window, fontSize);

  game.connectEventObservers(compositeEventObserver);

  game.start();

  uv_run(&loop, UV_RUN_DEFAULT);

  game.stop();

  uv_loop_close(&loop);

  return false;
}
