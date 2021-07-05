#pragma once

#include <graphics/gl/shader.hpp>

namespace my {
namespace graphics {
namespace gl {

class Program : public Object {
public:
  Program() : Object(glCreateProgram()) {}

  void attach_shaders(std::initializer_list<Shader> lst) {
    for (auto const &o : lst) {
      glAttachShader(*this, o);
    }
  }

  void link() { glLinkProgram(*this); }
  void use() { glUseProgram(*this); }

  ~Program() { glDeleteProgram(*this); }
};

} // namespace gl
} // namespace graphics
} // namespace my
