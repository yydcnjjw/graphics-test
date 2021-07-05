#pragma once

#include <type.hpp>
#include <glad/gl.h>
#include <platform/platform.hpp>

#include <window/window.hpp>

namespace my {

class App {
public:
  App() {}

  int run() {
    spdlog::set_level(spdlog::level::debug);

    platform::Window win{800, 600, "test", nullptr, nullptr};

    win.make_glctx();

    if (!gladLoadGL(glfwGetProcAddress)) {
      SPDLOG_ERROR("Failed to initialize GLAD");
      return -1;
    }

    glViewport(0, 0, 800, 600);

    while (!win.should_close()) {
      win.swapbuffer();
      ctx.poll_events();
    }

    return 0;
  }

  platform::glfw::Context ctx;
};

} // namespace my
