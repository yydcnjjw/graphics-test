#pragma once

#include <graphics/gl/shader.hpp>

namespace my {
namespace graphics {
namespace gl {

class Program : public Object {
public:
  Program() : Object(glCreateProgram()) {}

  void link() { glLinkProgram(*this); }
  void use() { glUseProgram(*this); }

  template <typename... Shader> void attach_shaders(Shader &&...shader) {
    for (auto o : {std::ref(shader)...}) {
      glAttachShader(*this, o.get());
    }
  }

  ~Program() { glDeleteProgram(*this); }
};

} // namespace gl
} // namespace graphics
} // namespace my
