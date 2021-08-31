#pragma once

#include <core/type.hpp>
#include <glm/gtx/quaternion.hpp>
#include <graphics/camera.hpp>
#include <graphics/gl/gl.hpp>
#include <imgui.hpp>
#include <platform/platform.hpp>
#include <stb_image.h>
#include <window/window.hpp>

namespace my {

struct Image {
  int width;
  int height;
  int nr_channels;
  u_char *data;

  ~Image() { stbi_image_free(data); }
};

struct ImageLoader {
  static Image load(std::string const &file) {
    Image image;
    image.data = stbi_load(file.c_str(), &image.width, &image.height,
                           &image.nr_channels, 0);

    if (!image.data) {
      throw std::runtime_error(
          (boost::format("load image %s failiure") % file).str());
    }
    return image;
  }
};

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
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)"}},
                   gl::Shader{gl::Shader::Type::kFragment, {R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}
)"}});
    program.link();

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    gl::Buffer vb(gl::Buffer::Target::kArray, gl::Buffer::Usage::kStaticDraw,
                  vertices, sizeof(vertices));
    // gl::Buffer eb(gl::Buffer::Target::kElementArray,
    //               gl::Buffer::Usage::kStaticDraw, indices, sizeof(indices));

    gl::VertexArray va;
    omap(
        [&] {
          // position attribute
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (void *)0);
          glEnableVertexAttribArray(0);
          // texture coord attribute
          glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                (void *)(3 * sizeof(float)));
          glEnableVertexAttribArray(1);
        },
        va, vb);

    stbi_set_flip_vertically_on_load(true);
    gl::Texture tex0{gl::Texture::Target::k2D};
    omap(
        [&] {
          auto image = ImageLoader::load("assets/container.jpg");
          tex0.load_image(0, GL_RGB, image.width, image.height, 0, GL_RGB,
                          GL_UNSIGNED_BYTE, image.data);
        },
        tex0);
    gl::Texture tex1{gl::Texture::Target::k2D};
    omap(
        [&] {
          auto image = ImageLoader::load("assets/awesomeface.png");
          tex1.load_image(0, GL_RGBA, image.width, image.height, 0, GL_RGBA,
                          GL_UNSIGNED_BYTE, image.data);
        },
        tex1);

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    auto projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    win.event<ev::FramebufferSize>().subscribe([&projection](auto e) {
      projection = glm::perspective(glm::radians(45.0f),
                                    static_cast<float>(e->width / e->height),
                                    0.1f, 100.0f);
    });

    graphics::Camera camera{};

    win.event<ev::Key>().subscribe([&camera](auto e) {
      SPDLOG_DEBUG("key: {}", e->key);

      switch (e->key) {
      case GLFW_KEY_W:
        camera.move(graphics::Camera::Movement::Forward, 1);
        break;
      case GLFW_KEY_S:
        camera.move(graphics::Camera::Movement::Backward, 1);
        break;
      case GLFW_KEY_A:
        camera.move(graphics::Camera::Movement::Left, 1);
        break;
      case GLFW_KEY_D:
        camera.move(graphics::Camera::Movement::Right, 1);
        break;
      }
    });

    double last_x{0}, last_y{0};
    bool first_move{true};

    win.event<ev::CursorPos>().subscribe([&](auto e) {
      auto xpos = e->xpos;
      auto ypos = e->ypos;
      if (first_move) {
        last_x = xpos;
        last_y = ypos;
        first_move = false;
      }

      float xoffset = (xpos - last_x) * 0.1f;
      float yoffset = (ypos - last_y) * 0.1f;

      last_x = xpos;
      last_y = ypos;

      SPDLOG_DEBUG("cursor offset: {}, {}", xoffset, yoffset);
      
      camera.euler_angle(xoffset, yoffset, 0.0f);
    });

    ImguiContext imgui_ctx(win);
    bool show_demo_window = true;

    glm::mat4 view{1.0};

    while (!win.should_close()) {
      ctx.poll_events();

      view = camera.look_at();

      imgui_ctx.clear();
      ImGui::ShowDemoWindow(&show_demo_window);
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      omap(
          [&] {
            program.uniform("texture1", 0);
            program.uniform("texture2", 1);

            auto model = glm::translate(glm::mat4{1.0}, glm::vec3{0, 0, 0});

            program.uniform("view", view);

            program.uniform("projection", projection);

            program.uniform("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
          },
          program, va, tex0[0], tex1[1]);

      imgui_ctx.render();

      win.swapbuffer();
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
