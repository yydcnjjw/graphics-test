#pragma once

#include <core/type.hpp>
#include <graphics/gl/object.hpp>

namespace my {
namespace graphics {
namespace gl {

class VertexArray : public Object {
public:
  VertexArray() : Object(gen_vertex_array()) {}

  ~VertexArray() {
    handle_t handle{*this};
    glDeleteVertexArrays(1, &handle);
  }

  void bind() { glBindVertexArray(*this); }

  template <typename CallBack> void map(CallBack &&cb) {
    bind();
    cb();
    unbind();
  }

  void unbind() { glBindVertexArray(0); }

private:
  static handle_t gen_vertex_array() {
    GLuint o;
    glGenVertexArrays(1, &o);
    return o;
  }
};

} // namespace gl
} // namespace graphics
} // namespace my
