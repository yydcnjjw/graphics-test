#pragma once

#include <backends/imgui_impl_glfw.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD2
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <platform/platform.hpp>

namespace my {

struct ImguiContext {
  ImguiContext(platform::Window &win) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(win, false);
    ImGui_ImplOpenGL3_Init("#version 330");

    win.event<ev::WindowFocus>().subscribe([&win](auto e) {
      ImGui_ImplGlfw_WindowFocusCallback(win, e->focused);
    });
    win.event<ev::CursorEnter>().subscribe([&win](auto e) {
      ImGui_ImplGlfw_CursorEnterCallback(win, e->focused);
    });
    win.event<ev::MouseButton>().subscribe([&win](auto e) {
      ImGui_ImplGlfw_MouseButtonCallback(win, e->button, e->action, e->mods);
    });
    win.event<ev::Scroll>().subscribe([&win](auto e) {
      ImGui_ImplGlfw_ScrollCallback(win, e->xoffset, e->yoffset);
    });
    win.event<ev::Key>().subscribe([&win](auto e) {
      ImGui_ImplGlfw_KeyCallback(win, e->key, e->scancode, e->action, e->mods);
    });
    win.event<ev::Char>().subscribe(
        [&win](auto e) { ImGui_ImplGlfw_CharCallback(win, e->ch); });
  }

  ~ImguiContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
  }

  void clear() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
  }

  void render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
};

} // namespace my
