#ifndef EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include <memory>
#include <mutex>
#include <queue>

#include "event.hpp"

namespace events {

class EventQueue {
public:
    EventQueue();

    void add(std::shared_ptr<Event> event);
    std::shared_ptr<Event> remove();

private:
    static bool priority_compare(const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b);

    std::mutex event_queue_mutex;
    std::priority_queue<std::shared_ptr<Event>, std::deque<std::shared_ptr<Event>>, decltype(&priority_compare)> event_queue;
};

}  // namespace events

#endif
