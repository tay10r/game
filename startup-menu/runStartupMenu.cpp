#include "runStartupMenu.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <vector>

bool
runStartupMenu(GLFWwindow* window)
{
  glClearColor(0, 0, 0, 1);

  std::vector<std::string> profiles;
  profiles.emplace_back("a");
  profiles.emplace_back("b");
  profiles.emplace_back("c");

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Select a profile.",
                 nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse);

    for (const std::string& profileName : profiles) {
      ImGui::Selectable(profileName.c_str());
    }

    ImGui::Button("New");

    ImGui::SameLine();

    ImGui::Button("Delete");

    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  return true;
}
