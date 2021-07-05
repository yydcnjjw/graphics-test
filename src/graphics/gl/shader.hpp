#pragma once

#include <graphics/gl/object.hpp>
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

    glShaderSource(*this, sources.size(), sources.data(), nullptr);
    compile();
  }

  ~Shader() { glDeleteShader(*this); }

  void compile() { glCompileShader(*this); }

private:
  Type _type;
};

} // namespace gl
} // namespace graphics
} // namespace my
