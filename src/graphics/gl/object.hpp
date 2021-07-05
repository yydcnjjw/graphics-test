#pragma once

#include <boost/noncopyable.hpp>
#include <glad/gl.h>

namespace my {
namespace graphics {
namespace gl {

struct Object : private boost::noncopyable {

  using handle_t = GLuint;

  Object(handle_t handle) : _handle(handle) {}

  operator handle_t() const { return _handle; }

private:
  handle_t _handle;
};

} // namespace gl
} // namespace graphics
} // namespace my
