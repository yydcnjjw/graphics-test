#pragma once

#include <glm/gtx/euler_angles.hpp>
#include <graphics/type.hpp>
#include <vector>

namespace my {
namespace graphics {

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
  enum class Movement { Forward, Backward, Left, Right };

  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;

  Camera() {}

  glm::mat4 look_at() { return glm::lookAt(position, position + front, up); }

  void move(Movement direction, float delta) {
    float velocity = delta;
    switch (direction) {
    case Movement::Forward: {
      position.z += velocity;
      break;
    }
    case Movement::Backward: {
      position.z -= velocity;
      break;
    }
    case Movement::Left: {
      position.x -= velocity;
      break;
    }
    case Movement::Right: {
      position.x += velocity;
      break;
    }
    }
  }

  template <typename... Args> void euler_angle(Args... args) {
    front *= glm::eulerAngleXYZ(std::forward<Args>(args)...);
  }
};

} // namespace graphics
} // namespace my
