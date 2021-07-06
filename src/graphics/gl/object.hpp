#pragma once

#include <boost/noncopyable.hpp>
#include <functional>
#include <glad/gl.h>
#include <tuple>
#include <type_traits>

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/algorithm/iteration.hpp>
#include <boost/fusion/include/iteration.hpp>
#include <boost/fusion/include/reverse_view.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/fusion/view/reverse_view.hpp>

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

template <typename Callback, typename... Object>
inline void omap(Callback &&cb, Object &&...o) {

  auto tuple = std::make_tuple(std::ref(o)...);
  boost::fusion::for_each(tuple, [](auto &o) { o.bind(); });
  cb();
  boost::fusion::for_each(boost::fusion::reverse_view<decltype(tuple)>{tuple},
                          [](auto &o) { o.unbind(); });
}

} // namespace gl
} // namespace graphics
} // namespace my
