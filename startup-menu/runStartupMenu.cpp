#include "runStartupMenu.h"

#include "../Profile.h"
#include "../Settings.h"

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <limits>
#include <string>
#include <vector>

#include <cstddef>

namespace {

class CachedProfileList final
{
public:
  CachedProfileList() { refresh(); }

  const std::vector<std::string>& paths() const { return m_paths; }

  const std::vector<std::string>& names() const { return m_names; }

  std::string path(size_t index) const noexcept
  {
    if (index >= m_paths.size())
      return "";
    else
      return m_paths[index];
  }

  std::string name(size_t index) const noexcept

  {
    if (index >= m_names.size())
      return "<null>";
    else
      return m_names[index].c_str();
  }

  size_t size() const noexcept { return m_paths.size(); }

  bool refresh()
  {
    m_paths = Settings::getProfileList();

    m_names.clear();

    for (const std::string& path : m_paths) {

      std::unique_ptr<Profile> profile = Settings::getProfile(path);
      if (profile)
        m_names.emplace_back(profile->name);
      else
        m_names.emplace_back("<null>");
    }

    return true;
  }

private:
  std::vector<std::string> m_paths;

  std::vector<std::string> m_names;
};

} // namespace

std::string
runStartupMenu(GLFWwindow* window, int fontSize)
{
  glClearColor(0, 0, 0, 1);

  size_t selectedProfileIndex = std::numeric_limits<size_t>::max();

  std::string renameBuffer;

  std::string selectedProfile;

  CachedProfileList profileList;

  while (!glfwWindowShouldClose(window) && selectedProfile.empty()) {

    int w = 0;
    int h = 0;
    glfwGetFramebufferSize(window, &w, &h);

    glViewport(0, 0, w, h);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    const ImVec2 windowPosition(w / 4, h / 4);

    const ImVec2 windowSize(w / 2, h / 2);

    ImGui::SetNextWindowPos(windowPosition);

    ImGui::SetNextWindowSize(windowSize);

    const ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Select a profile.", nullptr, windowFlags);

    for (size_t i = 0; i < profileList.size(); i++) {

      const bool selected = (i == selectedProfileIndex);

      if (ImGui::Selectable(profileList.name(i).c_str(), selected))
        selectedProfileIndex = i;
    }

    if (ImGui::Button("New")) {
      Settings::createProfile();
      profileList.refresh();
    }

    if (selectedProfileIndex < profileList.size()) {

      std::string profilePath = profileList.path(selectedProfileIndex);

      ImGui::SameLine();

      if (ImGui::Button("Delete")) {
        Settings::destroyProfile(profilePath);
        profileList.refresh();
      }

      ImGui::SameLine();

      if (ImGui::Button("Start"))
        selectedProfile = profileList.path(selectedProfileIndex);

      ImGui::SameLine();

      if (ImGui::Button("Rename")) {

        ImGui::OpenPopup("Rename Profile");

        renameBuffer.clear();

        std::unique_ptr<Profile> profile = Settings::getProfile(profileList.path(selectedProfileIndex));

        if (profile)
          renameBuffer = profile->name;
      }

      if (ImGui::BeginPopupModal("Rename Profile")) {

        ImGui::InputText("New Name", &renameBuffer);

        if (ImGui::Button("Save")) {

          if (Settings::renameProfile(profileList.path(selectedProfileIndex), renameBuffer))
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Cancel"))
          ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
      }
    }

    ImGui::Separator();

    if (ImGui::Button("Exit"))
      break;

    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  return selectedProfile;
}
