#pragma once

#include <boost/format.hpp>
#include <graphics/gl/shader.hpp>

namespace my {
namespace graphics {
namespace gl {

class Program : public Object {
public:
  Program() : Object(glCreateProgram()) {}

  void link() {
    glLinkProgram(*this);
    int result;
    glGetProgramiv(*this, GL_LINK_STATUS, &result);
    std::string buf(512, 0);
    if (!result) {
      glGetProgramInfoLog(*this, buf.size(), nullptr, buf.data());
      throw std::runtime_error(buf);
    }
  }

  void bind() { glUseProgram(*this); }
  void unbind() { glUseProgram(0); }

  template <typename... _Shader> void attach(_Shader &&...shader) {
    static_assert(
        std::conjunction_v<std::is_same<std::decay_t<_Shader>, Shader>...>, "");
    for (auto o : {std::ref(shader)...}) {
      glAttachShader(*this, o.get());
    }
  }

  template <typename... Args>
  void update_uniform(std::string const &name, Args &&...args) {
    auto location = glGetUniformLocation(*this, name.c_str());
    if (-1 == location) {
      throw std::runtime_error(
          (boost::format("Uniform %s does not exist") % name).str());
    }

    uniform(location, std::forward<Args>(args)...);
  }

  ~Program() { glDeleteProgram(*this); }

private:
  void uniform(int location, GLfloat v1) { glUniform1f(location, v1); }
  void uniform(int location, GLfloat v1, GLfloat v2) {
    glUniform2f(location, v1, v2);
  }
  void uniform(int location, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform3f(location, v1, v2, v3);
  }
  void uniform(int location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) {
    glUniform4f(location, v1, v2, v3, v4);
  }

  void uniform(int location, GLuint v1) { glUniform1ui(location, v1); }
  void uniform(int location, GLuint v1, GLuint v2) {
    glUniform2ui(location, v1, v2);
  }
  void uniform(int location, GLuint v1, GLuint v2, GLuint v3) {
    glUniform3ui(location, v1, v2, v3);
  }
  void uniform(int location, GLuint v1, GLuint v2, GLuint v3, GLuint v4) {
    glUniform4ui(location, v1, v2, v3, v4);
  }

  void uniform(int location, GLint v1) { glUniform1i(location, v1); }
  void uniform(int location, GLint v1, GLint v2) {
    glUniform2i(location, v1, v2);
  }
  void uniform(int location, GLint v1, GLint v2, GLint v3) {
    glUniform3i(location, v1, v2, v3);
  }
  void uniform(int location, GLint v1, GLint v2, GLint v3, GLint v4) {
    glUniform4i(location, v1, v2, v3, v4);
  }
};

} // namespace gl
} // namespace graphics
} // namespace my
