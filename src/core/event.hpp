#pragma once

#include <typeindex>

#include <core/type.hpp>

namespace my {

class IEvent {
  using clock_type = std::chrono::system_clock;
  using time_point = clock_type::time_point;

public:
  time_point const timestamp;

  IEvent(std::type_info const &type)
      : timestamp(clock_type::now()), _type(type) {}

  virtual ~IEvent() = default;
  template <typename T> bool is() { return this->_type == typeid(T); }

private:
  std::type_info const &_type;
};

using dynamic_event_t = std::shared_ptr<IEvent>;

template <typename EventType>
struct Event : public IEvent, std::enable_shared_from_this<Event<EventType>> {
  using event_t = EventType;

  Event() : IEvent{typeid(event_t)} {}

  dynamic_event_t as_dynamic() {
    return std::reinterpret_pointer_cast<IEvent>(this->shared_from_this());
  }
};

template <typename T,
          typename = std::enable_if_t<std::is_base_of_v<Event<T>, T>>>
inline decltype(auto) on_event(rx::observable<dynamic_event_t> o) {
  return o.filter([](auto e) { return e->template is<T>(); }).map([](auto e) {
    return std::dynamic_pointer_cast<T>(e);
  });
}

template <typename T, typename Observer, typename... Args>
inline void post_event(Observer const &ob, Args &&...args) {
  ob.on_next(std::make_shared<T>(std::forward<Args>(args)...));
}

} // namespace my
