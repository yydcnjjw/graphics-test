#pragma once

#include <type_traits>

namespace my {
namespace window {

template <typename WindowType, typename = void>
class traits : std::false_type {};

template <typename WindowType>
class traits<WindowType, std::void_t<typename WindowType::handle_type>>
    : std::true_type {
  using handle_type = typename WindowType::handle_type;
};

} // namespace window
} // namespace my
