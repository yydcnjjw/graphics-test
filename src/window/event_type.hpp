#pragma once

#include <boost/hana.hpp>
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

struct MouseButton : Event<MouseButton> {
  int button;
  int action;
  int mods;

  MouseButton(int button, int action, int mods)
      : button(button), action(action), mods(mods) {}
};

struct WindowFocus : Event<WindowFocus> {
  bool focused;
  WindowFocus(bool focused) : focused(focused) {}
};

struct CursorEnter : Event<CursorEnter> {
  bool focused;
  CursorEnter(bool focused) : focused(focused) {}
};

struct Scroll : Event<Scroll> {
  double xoffset;
  double yoffset;
  Scroll(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
};

struct Char : Event<Char> {
  unsigned int ch;
  Char(unsigned int ch) : ch(ch) {}
};

struct Monitor : Event<Monitor> {
  int event;
  Monitor(int event) : event(event) {}
};

} // namespace ev
} // namespace my
