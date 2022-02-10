#ifndef EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include <mutex>
#include <optional>
#include <queue>

#include "event.hpp"

namespace events {

class EventQueue {
public:
    EventQueue();

    void add(Event event);
    std::optional<Event> remove();

private:
    static bool priority_compare(const Event& a, const Event& b);

    std::mutex event_queue_mutex;
    std::priority_queue<Event, std::deque<Event>, decltype(&priority_compare)> event_queue;
};

}  // namespace events

#endif
