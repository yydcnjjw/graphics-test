#pragma once

#include <core/type.hpp>
#include <glad/gl.h>
#include <graphics/gl/gl.hpp>
#include <platform/platform.hpp>

#include <window/window.hpp>

namespace my {

class App {
public:
  App() {}

  int exec() {
    SPDLOG_DEBUG("application running");
    platform::Window win{800, 600, "test", nullptr, nullptr};

    win.make_glctx();

    win.event<ev::FramebufferSize>().subscribe([](auto e) {
      SPDLOG_DEBUG("framebuffer size: {} {}", e->width, e->height);
      glViewport(0, 0, e->width, e->height);
    });

    win.event<ev::Key>().subscribe([&win](auto e) {
      if (e->key == GLFW_KEY_ESCAPE && e->action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, true);
      }
    });

    if (!gladLoadGL(glfwGetProcAddress)) {
      SPDLOG_ERROR("Failed to initialize GLAD");
      return -1;
    }

    namespace gl = graphics::gl;

    gl::Program program;
    program.attach(gl::Shader{gl::Shader::Type::kVertex, {R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
)"}},
                   gl::Shader{gl::Shader::Type::kFragment, {R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0);
}
)"}});
    program.link();

    float vertices[] = {
        // 位置              // 颜色
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // 顶部
    };

    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    gl::Buffer vb(gl::Buffer::Target::kArray, gl::Buffer::Usage::kStaticDraw,
                  vertices, sizeof(vertices));
    gl::Buffer eb(gl::Buffer::Target::kElementArray,
                  gl::Buffer::Usage::kStaticDraw, indices, sizeof(indices));

    gl::VertexArray va;
    omap(
        [&]() {
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                                (void *)0);
          glEnableVertexAttribArray(0);

          glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                                (void *)(3 * sizeof(float)));
          glEnableVertexAttribArray(1);
        },
        va, vb);

    glViewport(0, 0, 800, 600);

    while (!win.should_close()) {

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      omap(
          [&]() {
            glDrawArrays(GL_TRIANGLES, 0, 3);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
          },
          program, va, eb);

      win.swapbuffer();
      ctx.poll_events();
    }

    SPDLOG_DEBUG("application quit");

    return 0;
  }

  static int run() {
    spdlog::set_level(spdlog::level::debug);

    return App{}.exec();
  }

  platform::Context ctx;
};

} // namespace my
