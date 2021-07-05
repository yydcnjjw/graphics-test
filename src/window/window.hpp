#pragma once

#include <type_traits>

namespace my {
namespace window {

template <typename WindowType, typename = void>
class traits : std::false_type {};

template <typename WindowType>
class traits<WindowType, std::void_t<typename WindowType::handle_t>>
    : std::true_type {
  using handle_t = typename WindowType::handle_t;
};

} // namespace window
} // namespace my
