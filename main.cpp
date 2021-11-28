#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Settings.h"

#include "GLFWEventObserver.h"
#include "play/play.h"
#include "startup-menu/runStartupMenu.h"

#ifdef _WIN32
INT WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, INT)
#else
int
main()
#endif
{
  if (!Settings::init()) {
    std::cerr << "Failed to create settings directory." << std::endl;
    return EXIT_FAILURE;
  }

  if (glfwInit() != GLFW_TRUE) {
    std::cerr << "Failed to initialize GLFW." << std::endl;
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

  GLFWwindow* window = glfwCreateWindow(1280, 720, PROJECT_NAME, glfwGetPrimaryMonitor(), nullptr);

  if (!window) {
    std::cerr << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

  glfwSwapInterval(1);

  CompositeGLFWEventObserver compositeEventObserver;

  GLFWEventObserver::install(window, &compositeEventObserver);

  WindowViewportResizer viewportResizer;

  compositeEventObserver.addEventObserver(&viewportResizer);

  int w = 0;
  int h = 0;
  glfwGetFramebufferSize(window, &w, &h);
  glViewport(0, 0, w, h);

  float xScale = 1.0f;
  float yScale = 1.0f;
  glfwGetWindowContentScale(window, &xScale, &yScale);

  const float fontSize = 24 * xScale;

  IMGUI_CHECKVERSION();

  ImGui::CreateContext();

  ImGui_ImplGlfw_InitForOpenGL(window, false);

  ImGui_ImplOpenGL3_Init("#version 300 es");

  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF(ASSETS_DIR "/fonts/Roboto-Medium.ttf", fontSize);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowTitleAlign = ImVec2(0.5, 0.5);
  style.FrameRounding = 5 * xScale;
  style.ScrollbarRounding = 5 * xScale;
  style.WindowRounding = 5 * xScale;
  style.ItemSpacing = ImVec2(5 * xScale, 5 * yScale);

  ImGuiProxy imGuiProxy(window);

  compositeEventObserver.addEventObserver(&imGuiProxy);

  const std::string selectedProfile = runStartupMenu(window, fontSize);

  int exitCode = EXIT_SUCCESS;

  if (!selectedProfile.empty()) {
    exitCode = play(window, &compositeEventObserver, selectedProfile.c_str(), fontSize) ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  ImGui_ImplOpenGL3_Shutdown();

  ImGui_ImplGlfw_Shutdown();

  ImGui::DestroyContext();

  glfwDestroyWindow(window);

  glfwTerminate();

  return exitCode;
}
