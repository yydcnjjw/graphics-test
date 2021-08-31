#pragma once

#include <core/observable.hpp>
#include <window/event_type.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <boost/format.hpp>
#include <boost/noncopyable.hpp>
#include <utility>

namespace my {
namespace platform {
namespace glfw {

static inline auto err_str() {
  char const *str;
  glfwGetError(&str);
  return str;
}

static inline auto err_code() { return glfwGetError(nullptr); }

class Context {
public:
  Context() {
    if (glfwInit() == GLFW_FALSE) {
      throw std::runtime_error(err_str());
    }

    // TODO: Can be prepared
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(&Context::error_cb);
  }

  void poll_events() { glfwPollEvents(); }

  ~Context() { glfwTerminate(); }

private:
  static void error_cb(int error, const char *description) {
    SPDLOG_WARN(description);
  }
};

class Window : private boost::noncopyable, public Observable {
public:
  using handle_t = GLFWwindow *;

  template <typename... Args> Window(Args &&...args) {
    _handle = glfwCreateWindow(std::forward<Args>(args)...);
    if (!_handle) {
      throw std::runtime_error(err_str());
    }

    glfwSetWindowUserPointer(*this, this);

    glfwSetFramebufferSizeCallback(*this, &Window::framebuffer_size_cb);
    glfwSetKeyCallback(*this, &Window::key_cb);
    glfwSetWindowFocusCallback(*this, &Window::window_focus_cb);
    glfwSetCursorEnterCallback(*this, &Window::cursor_enter_cb);
    glfwSetMouseButtonCallback(*this, &Window::mouse_button_cb);
    glfwSetScrollCallback(*this, &Window::scroll_cb);
    glfwSetCharCallback(*this, &Window::char_cb);
    glfwSetCursorPosCallback(*this, &Window::cursor_pos);
  }

  ~Window() { glfwDestroyWindow(_handle); }

  auto should_close() { return glfwWindowShouldClose(*this); }

  void swapbuffer() { glfwSwapBuffers(*this); }

  void make_glctx() { glfwMakeContextCurrent(*this); }

  operator handle_t() { return _handle; }

private:
  handle_t _handle;

  static Window *user_pointer(handle_t handle) {
    return reinterpret_cast<Window *>(glfwGetWindowUserPointer(handle));
  }

  template <typename EventType, typename... Args>
  static void post_inner(handle_t handle, Args &&...args) {
    user_pointer(handle)->post<EventType>(std::forward<Args>(args)...);
  }

  static void framebuffer_size_cb(handle_t handle, int w, int h) {
    post_inner<ev::FramebufferSize>(handle, w, h);
  }
  static void key_cb(handle_t handle, int key, int scancode, int action,
                     int mods) {
    post_inner<ev::Key>(handle, key, scancode, action, mods);
  }
  static void window_focus_cb(handle_t handle, int focused) {
    post_inner<ev::WindowFocus>(handle, focused);
  }
  static void cursor_enter_cb(handle_t handle, int focused) {
    post_inner<ev::WindowFocus>(handle, focused);
  }
  static void mouse_button_cb(handle_t handle, int button, int action,
                              int mods) {
    post_inner<ev::MouseButton>(handle, button, action, mods);
  }
  static void scroll_cb(handle_t handle, double xoffset, double yoffset) {
    post_inner<ev::Scroll>(handle, xoffset, yoffset);
  }
  static void char_cb(handle_t handle, unsigned int c) {
    post_inner<ev::Char>(handle, c);
  }
  static void cursor_pos(handle_t handle, double xpos, double ypos) {
    post_inner<ev::CursorPos>(handle, xpos, ypos);
  }
};

} // namespace glfw
} // namespace platform
} // namespace my
