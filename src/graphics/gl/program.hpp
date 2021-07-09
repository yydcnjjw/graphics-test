#pragma once

#include <boost/format.hpp>
#include <graphics/gl/shader.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/type.hpp>

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
  void uniform(std::string const &name, Args &&...args) {
    auto location = glGetUniformLocation(*this, name.c_str());
    if (-1 == location) {
      throw std::runtime_error(
          (boost::format("Uniform %s does not exist") % name).str());
    }

    _uniform(location, std::forward<Args>(args)...);
  }

  ~Program() { glDeleteProgram(*this); }

private:
  void _uniform(int location, GLfloat v1) { glUniform1f(location, v1); }
  void _uniform(int location, GLfloat v1, GLfloat v2) {
    glUniform2f(location, v1, v2);
  }
  void _uniform(int location, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform3f(location, v1, v2, v3);
  }
  void _uniform(int location, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4) {
    glUniform4f(location, v1, v2, v3, v4);
  }

  void _uniform(int location, GLuint v1) { glUniform1ui(location, v1); }
  void _uniform(int location, GLuint v1, GLuint v2) {
    glUniform2ui(location, v1, v2);
  }
  void _uniform(int location, GLuint v1, GLuint v2, GLuint v3) {
    glUniform3ui(location, v1, v2, v3);
  }
  void _uniform(int location, GLuint v1, GLuint v2, GLuint v3, GLuint v4) {
    glUniform4ui(location, v1, v2, v3, v4);
  }

  void _uniform(int location, GLint v1) { glUniform1i(location, v1); }
  void _uniform(int location, GLint v1, GLint v2) {
    glUniform2i(location, v1, v2);
  }
  void _uniform(int location, GLint v1, GLint v2, GLint v3) {
    glUniform3i(location, v1, v2, v3);
  }

  void _uniform(int location, GLint v1, GLint v2, GLint v3, GLint v4) {
    glUniform4i(location, v1, v2, v3, v4);
  }

  template <typename Buffer, typename = void>
  struct is_buffer : std::false_type {};
  template <typename Buffer>
  struct is_buffer<Buffer, std::void_t<decltype(boost::asio::buffer(
                               std::declval<Buffer>()))>> : std::true_type {};

  template <typename Buffer, typename ValueType, typename = void>
  struct is_buffer_value : std::false_type {};
  template <typename Buffer, typename ValueType>
  struct is_buffer_value<Buffer, ValueType,
                         std::void_t<std::iter_value_t<Buffer>>>
      : std::conjunction<is_buffer<Buffer>,
                         std::is_same<std::iter_value_t<Buffer>, ValueType>> {};

  template <typename T, typename ValueType>
  struct is_value_or_buffer : std::disjunction<std::is_same<T, ValueType>,
                                               is_buffer_value<T, ValueType>> {
  };

  template <typename NativeVecFn, typename Buffer>
  void _uniform_vec(NativeVecFn fn, int location, Buffer const &buf,
                    std::enable_if_t<is_buffer<Buffer>::value> *_ = nullptr) {

    using ValueType = std::iter_value_t<Buffer>;
    using T = decltype(glm::value_ptr(std::declval<ValueType>()));
    auto b = boost::asio::buffer(buf);
    fn(location, b.size() / sizeof(ValueType), reinterpret_cast<T>(b.data()));
  }

  template <typename NativeVecFn, typename ValueType>
  void
  _uniform_vec(NativeVecFn fn, int location, ValueType const &val,
               std::enable_if_t<!is_buffer<ValueType>::value> *_ = nullptr) {

    fn(location, 1, glm::value_ptr(val));
  }

  template <typename NativeMatFn, typename Buffer>
  void _uniform_mat(NativeMatFn fn, int location, Buffer const &buf,
                    GLboolean transpose,
                    std::enable_if_t<is_buffer<Buffer>::value> *_ = nullptr) {

    using ValueType = std::iter_value_t<Buffer>;
    using T = decltype(glm::value_ptr(std::declval<ValueType>()));
    auto b = boost::asio::buffer(buf);
    fn(location, b.size() / sizeof(ValueType), transpose,
       reinterpret_cast<T>(b.data()));
  }

  template <typename NativeMatFn, typename ValueType>
  void
  _uniform_mat(NativeMatFn fn, int location, ValueType const &val,
               GLboolean transpose,
               std::enable_if_t<!is_buffer<ValueType>::value> *_ = nullptr) {

    fn(location, 1, transpose, glm::value_ptr(val));
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::fvec1>::value> *_ = nullptr) {
    _uniform_vec(glUniform1fv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::fvec2>::value> *_ = nullptr) {
    _uniform_vec(glUniform2fv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::fvec3>::value> *_ = nullptr) {
    _uniform_vec(glUniform3fv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::fvec4>::value> *_ = nullptr) {
    _uniform_vec(glUniform4fv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::ivec1>::value> *_ = nullptr) {
    _uniform_vec(glUniform1iv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::ivec2>::value> *_ = nullptr) {
    _uniform_vec(glUniform2iv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::ivec3>::value> *_ = nullptr) {
    _uniform_vec(glUniform3iv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::ivec4>::value> *_ = nullptr) {
    _uniform_vec(glUniform4iv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::uvec1>::value> *_ = nullptr) {
    _uniform_vec(glUniform1uiv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::uvec2>::value> *_ = nullptr) {
    _uniform_vec(glUniform2uiv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::uvec3>::value> *_ = nullptr) {
    _uniform_vec(glUniform3uiv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t,
      std::enable_if_t<is_value_or_buffer<T, glm::uvec4>::value> *_ = nullptr) {
    _uniform_vec(glUniform4uiv, location, t);
  }

  template <typename T>
  void _uniform(
      int location, T const t, GLboolean transpose = GL_FALSE,
      std::enable_if_t<is_value_or_buffer<T, glm::mat2>::value> *_ = nullptr) {
    _uniform_mat(glUniformMatrix2fv, location, t, transpose);
  }

  template <typename T>
  void _uniform(
      int location, T const t, GLboolean transpose = GL_FALSE,
      std::enable_if_t<is_value_or_buffer<T, glm::mat3>::value> *_ = nullptr) {
    _uniform_mat(glUniformMatrix3fv, location, t, transpose);
  }

  template <typename T>
  void _uniform(
      int location, T const t, GLboolean transpose = GL_FALSE,
      std::enable_if_t<is_value_or_buffer<T, glm::mat4>::value> *_ = nullptr) {
    _uniform_mat(glUniformMatrix4fv, location, t, transpose);
  }

  template <typename T>
  void _uniform(int location, T const t, GLboolean transpose = GL_FALSE,
                std::enable_if_t<is_value_or_buffer<T, glm::mat2x3>::value> *_ =
                    nullptr) {
    _uniform_mat(glUniformMatrix2x3fv, location, t, transpose);
  }

  template <typename T>
  void _uniform(int location, T const t, GLboolean transpose = GL_FALSE,
                std::enable_if_t<is_value_or_buffer<T, glm::mat3x2>::value> *_ =
                    nullptr) {
    _uniform_mat(glUniformMatrix3x2fv, location, t, transpose);
  }

  template <typename T>
  void _uniform(int location, T const t, GLboolean transpose = GL_FALSE,
                std::enable_if_t<is_value_or_buffer<T, glm::mat2x4>::value> *_ =
                    nullptr) {
    _uniform_mat(glUniformMatrix2x4fv, location, t, transpose);
  }

  template <typename T>
  void _uniform(int location, T const t, GLboolean transpose = GL_FALSE,
                std::enable_if_t<is_value_or_buffer<T, glm::mat4x2>::value> *_ =
                    nullptr) {
    _uniform_mat(glUniformMatrix4x2fv, location, t, transpose);
  }

  template <typename T>
  void _uniform(int location, T const t, GLboolean transpose = GL_FALSE,
                std::enable_if_t<is_value_or_buffer<T, glm::mat3x4>::value> *_ =
                    nullptr) {
    _uniform_mat(glUniformMatrix3x4fv, location, t, transpose);
  }

  template <typename T>
  void _uniform(int location, T const t, GLboolean transpose = GL_FALSE,
                std::enable_if_t<is_value_or_buffer<T, glm::mat4x3>::value> *_ =
                    nullptr) {
    _uniform_mat(glUniformMatrix4x3fv, location, t, transpose);
  }
};

} // namespace gl
} // namespace graphics
} // namespace my
