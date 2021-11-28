#include "play.h"

#include "Game.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <uv.h>

#include <sstream>

namespace {

void
displayShaderErrors(GLFWwindow* window, const std::string& errorLog)
{
  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT);

    int w = 0;
    int h = 0;
    glfwGetFramebufferSize(window, &w, &h);

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(w / 4, h / 4));

    ImGui::SetNextWindowSize(ImVec2(w / 2, h / 2));

    const ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Shader Error Log");

    ImGui::Text(errorLog.c_str(), nullptr, windowFlags);

    if (ImGui::Button("Exit"))
      break;

    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwPollEvents();

    glfwSwapBuffers(window);
  }
}

} // namespace

bool
play(GLFWwindow* window, CompositeGLFWEventObserver* compositeEventObserver, const char* profilePath, int fontSize)
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  uv_loop_t loop;

  uv_loop_init(&loop);

  Game game(&loop, window, fontSize);

  std::ostringstream shaderErrorLog;

  if (!game.compileShaders(shaderErrorLog)) {
    displayShaderErrors(window, shaderErrorLog.str());
    uv_loop_close(&loop);
    return false;
  }

  game.connectEventObservers(compositeEventObserver);

  game.start();

  uv_run(&loop, UV_RUN_DEFAULT);

  game.stop();

  uv_loop_close(&loop);

  return false;
}
