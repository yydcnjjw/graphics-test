#pragma once

#include <core/event.hpp>

namespace my {
namespace ev {

struct Quit : Event<Quit> {
  int quit_code{0};
};

} // namespace ev

} // namespace my
