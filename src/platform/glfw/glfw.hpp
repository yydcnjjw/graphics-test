#pragma once

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

class Window : private boost::noncopyable {
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
  }

  ~Window() { glfwDestroyWindow(_handle); }

  auto should_close() { return glfwWindowShouldClose(*this); }

  void swapbuffer() { glfwSwapBuffers(*this); }

  void make_glctx() { glfwMakeContextCurrent(*this); }

  operator handle_t() { return _handle; }

  template <typename EventType> auto event() {
    return on_event<EventType>(_subject.get_observable());
  }

private:
  handle_t _handle;
  rx::subjects::subject<dynamic_event_t> _subject;

  template <typename EventType, typename... Args> void post(Args &&...args) {
    post_event<EventType>(_subject.get_subscriber(),
                          std::forward<Args>(args)...);
  }

  static Window *user_pointer(handle_t handle) {
    return reinterpret_cast<Window *>(glfwGetWindowUserPointer(handle));
  }

  template <typename EventType, typename... Args>
  static void post(handle_t handle, Args &&...args) {
    user_pointer(handle)->post<EventType>(std::forward<Args>(args)...);
  }

  static void framebuffer_size_cb(handle_t handle, int w, int h) {
    post<ev::FramebufferSize>(handle, w, h);
  }
  static void key_cb(handle_t handle, int key, int scancode, int action,
                     int mods) {
    post<ev::Key>(handle, key, scancode, action, mods);
  }
};

} // namespace glfw
} // namespace platform
} // namespace my
