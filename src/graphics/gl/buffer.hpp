#pragma once

#include <core/type.hpp>
#include <graphics/gl/object.hpp>

namespace my {
namespace graphics {
namespace gl {

class Buffer : public Object {
public:
  enum class Target : GLenum {
    kArray = GL_ARRAY_BUFFER,
    kElementArray = GL_ELEMENT_ARRAY_BUFFER
  };

  enum class Usage : GLenum { kStaticDraw = GL_STATIC_DRAW };

  Buffer(Target target, Usage usage, void *data, std::size_t size)
      : Object(gen_buffer()), _target(target) {

    omap(
        [&]() {
          glBufferData(static_cast<GLenum>(target), size, data,
                       static_cast<GLenum>(usage));
        },
        *this);
  }

  void bind() { glBindBuffer(static_cast<GLenum>(_target), *this); }

  void unbind() { glBindBuffer(static_cast<GLenum>(_target), 0); }

  ~Buffer() {
    handle_t handle{*this};
    glDeleteBuffers(1, &handle);
  }

private:
  Target _target;

  static handle_t gen_buffer() {
    GLuint o;
    glGenBuffers(1, &o);
    return o;
  }
};

} // namespace gl
} // namespace graphics
} // namespace my
