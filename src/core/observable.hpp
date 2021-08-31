#pragma once

#include <core/event.hpp>

namespace my {
class Observable {
public:
  template <typename EventType> auto event() {
    return on_event<EventType>(_subject.get_observable());
  }

protected:
  template <typename EventType, typename... Args> void post(Args &&...args) {
    post_event<EventType>(_subject.get_subscriber(),
                          std::forward<Args>(args)...);
  }

private:
  rx::subjects::subject<dynamic_event_t> _subject;
};

} // namespace my
