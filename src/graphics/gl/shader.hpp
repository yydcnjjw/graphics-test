#pragma once

#include <graphics/gl/object.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace my {
namespace graphics {
namespace gl {

class Shader : public Object {
public:
  enum class Type : GLenum {
    kVertex = GL_VERTEX_SHADER,
    kFragment = GL_FRAGMENT_SHADER
  };

  Shader(Type type, std::vector<char const *> const &sources)
      : Object(glCreateShader(static_cast<GLenum>(type))), _type(type) {

    glShaderSource(*this, static_cast<GLsizei>(sources.size()), sources.data(), nullptr);
    compile();
   }

  ~Shader() { glDeleteShader(*this); }

  void compile() {
    glCompileShader(*this);
    int result;
    glGetShaderiv(*this, GL_COMPILE_STATUS, &result);
    std::string buf(512, 0);
    if (!result) {
      glGetShaderInfoLog(*this, buf.size(), nullptr, buf.data());
      throw std::runtime_error(buf);
    }
  }

private:
  Type _type;
};

} // namespace gl
} // namespace graphics
} // namespace my
