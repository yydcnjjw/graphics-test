#pragma once

#include <glm/gtx/euler_angles.hpp>
#include <graphics/type.hpp>
#include <vector>

namespace my {
namespace graphics {

class Camera {
public:
  enum class Movement { Forward, Backward, Left, Right };

  Camera(glm::vec3 const &position = {}, glm::vec3 const &front = {0, 0, -1},
         glm::vec3 const &up = {0, 1, 0})
      : _position(position), _front(front), _up(up) {}

  glm::mat4 look_at() {
    return glm::lookAt(_position, _position + _front, _up);
  }

  void move(Movement direction, float delta) {
    float velocity = delta;
    switch (direction) {
    case Movement::Forward: {
      _position.z -= velocity;
      break;
    }
    case Movement::Backward: {
      _position.z += velocity;
      break;
    }
    case Movement::Left: {
      _position.x -= velocity;
      break;
    }
    case Movement::Right: {
      _position.x += velocity;
      break;
    }
    }
  }

  template <typename... Args> void euler_angle(Args... args) {
    _front = glm::vec4(_front, 1.0) * glm::eulerAngleXYZ(std::forward<Args>(args)...);
  }

private:
  glm::vec3 _position;
  glm::vec3 _front;
  glm::vec3 _up;
};

} // namespace graphics
} // namespace my
