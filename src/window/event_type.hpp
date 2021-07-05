#pragma once

#include <core/event.hpp>

namespace my {
namespace ev {

struct FramebufferSize : Event<FramebufferSize> {
  int width;
  int height;

  FramebufferSize(int width, int height) : width(width), height(height) {}
};

struct Key : Event<Key> {
  int key;
  int scancode;
  int action;
  int mods;

  Key(int key, int scancode, int action, int mods)
      : key(key), scancode(scancode), action(action), mods(mods) {}
};

} // namespace ev
} // namespace my
