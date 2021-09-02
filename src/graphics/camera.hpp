#pragma once

#include <graphics/type.hpp>
#include <platform/platform.hpp>
#include <vector>

namespace my {
namespace graphics {

class Camera {
public:
  struct EulerAngule {
    float yaw;
    float pitch;
    float roll;

    glm::vec3 front() const {
      auto yaw = glm::radians(this->yaw);
      auto pitch = glm::radians(this->pitch);

      return glm::normalize(
          glm::vec3{cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)});
    }
  };

  Camera(glm::vec2 viewport, glm::vec3 const &pos = {})
      : _viewport(viewport), _pos(pos) {}

  auto look_at() const {
    auto front = _euler_angule.front();
    auto right = glm::normalize(glm::cross(front, _world_up));
    auto up = glm::normalize(glm::cross(right, front));
    return glm::lookAt(_pos, _pos + front, up);
  }

  auto perspective() const {
    return glm::perspective(glm::radians(fov()), _viewport.x / _viewport.y,
                            0.1f, 100.0f);
  }

  glm::vec2 viewport() const { return _viewport; }
  void viewport(glm::vec2 const &viewport) { _viewport = viewport; }

  void pos(glm::vec3 const &pos) { _pos = pos; }
  glm::vec3 pos() const { return _pos; }

  void euler_angle(EulerAngule const &v) { _euler_angule = v; }
  EulerAngule euler_angle() const { return _euler_angule; }

  float fov() const { return _fov; }
  void fov(float v) { _fov = v; }

private:
  glm::vec2 _viewport;
  glm::vec3 _pos;
  EulerAngule _euler_angule;
  float _fov{45};

  glm::vec3 _world_up{0, 1, 0};
};

struct CameraController {
  float last_x{0}, last_y{0};
  bool first_move{true};

  CameraController(platform::Window &win, Camera &camera) {
    win.event<ev::Key>().subscribe([&camera](auto e) {
      auto pos = camera.pos();
      auto delta = 0.5;
      switch (e->key) {
      case GLFW_KEY_W:
        pos.x += delta;
        break;
      case GLFW_KEY_S:
        pos.x -= delta;
        break;
      case GLFW_KEY_A:
        pos.z -= delta;
        break;
      case GLFW_KEY_D:
        pos.z += delta;
        break;
      }
      camera.pos(pos);
    });

    win.event<ev::CursorPos>().subscribe([&](auto e) {
      float xpos = e->xpos;
      float ypos = e->ypos;
      if (first_move) {
        last_x = xpos;
        last_y = ypos;
        first_move = false;
      }

      float xoffset = (xpos - last_x) * 0.05f;
      float yoffset = (ypos - last_y) * 0.05f;

      last_x = xpos;
      last_y = ypos;

      auto ea = camera.euler_angle();
      ea.yaw += xoffset;
      ea.pitch = yoffset;
      camera.euler_angle(ea);
    });

    win.event<ev::FramebufferSize>().subscribe([&camera](auto e) {
      camera.viewport({e->width, e->height});
    });

    win.event<ev::Scroll>().subscribe([&camera](auto e) {
      auto fov = camera.fov();
      if (fov >= 1.0f && fov <= 45.0f)
        fov -= static_cast<float>(e->yoffset);
      if (fov <= 1.0f)
        fov = 1.0f;
      if (fov >= 45.0f)
        fov = 45.0f;
      camera.fov(fov);
    });
  }
};

} // namespace graphics
} // namespace my
