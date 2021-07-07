#pragma once

#include <core/type.hpp>
#include <graphics/gl/object.hpp>

namespace my {
namespace graphics {
namespace gl {

class Texture : public Object {
public:
  enum class Target : GLenum {
    k2D = GL_TEXTURE_2D,
  };

  enum class WrapType : GLenum {
    kClampToEdge = GL_CLAMP_TO_EDGE,
    kClampToBorder = GL_CLAMP_TO_BORDER,
    kMirroredRepeat = GL_MIRRORED_REPEAT,
    kRepeat = GL_REPEAT,
    kMirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE
  };

  enum class FilterType : GLenum { kNearest = GL_NEAREST, kLinear = GL_LINEAR };

  Texture(Target target) : Object(gen_texture()), _target(target) {}

  void bind() { glBindTexture(static_cast<GLenum>(_target), *this); }

  void unbind() { glBindTexture(static_cast<GLenum>(_target), 0); }

  void min_filter(FilterType type) {
    tex_parameter(GL_TEXTURE_MIN_FILTER, static_cast<GLint>(type));
  }

  void mag_filter(FilterType type) {
    assert(type == FilterType::kNearest || type == FilterType::kLinear);
    tex_parameter(GL_TEXTURE_MAG_FILTER, static_cast<GLint>(type));
  }

  void wrap(WrapType s = WrapType::kRepeat, WrapType t = WrapType::kRepeat,
            WrapType r = WrapType::kRepeat) {
    tex_parameter(GL_TEXTURE_WRAP_S, static_cast<GLint>(s));
    tex_parameter(GL_TEXTURE_WRAP_T, static_cast<GLint>(t));
    tex_parameter(GL_TEXTURE_WRAP_R, static_cast<GLint>(r));
  }

  void load_image(GLint level, GLint internalformat, GLsizei width,
                  GLsizei height, GLint border, GLenum format, GLenum type,
                  const void *pixels) {
    glTexImage2D(static_cast<GLenum>(_target), level, internalformat, width,
                 height, border, format, type, pixels);
    glGenerateMipmap(static_cast<GLenum>(_target));
  }

  ~Texture() {
    handle_t handle{*this};
    glDeleteTextures(1, &handle);
  }

private:
  Target _target;

  static handle_t gen_texture() {
    GLuint o;
    glGenTextures(1, &o);
    return o;
  }

  void tex_parameter(GLenum name, GLint v) {
    glTexParameteri(static_cast<GLenum>(_target), name, v);
  }

  void tex_parameter(GLenum name, GLfloat v) {
    glTexParameterf(static_cast<GLenum>(_target), name, v);
  }
};

} // namespace gl
} // namespace graphics
} // namespace my
