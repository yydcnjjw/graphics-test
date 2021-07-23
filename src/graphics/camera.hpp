#pragma once

#include <graphics/type.hpp>
#include <glm/ext/quaternion_transform.hpp>

namespace my {
namespace graphics {

class Camera {
public:
  template <typename... Args> static auto lookat(Args &&...args) {
    return glm::lookAt(std::forward<Args>(args)...);
  }

private:
  glm::vec3 _front;
  glm::vec4 _projection;

  void rotate() {

  }
};

} // namespace graphics
} // namespace my
