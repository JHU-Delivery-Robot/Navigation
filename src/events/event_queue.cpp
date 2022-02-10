#include "event_queue.hpp"

#include <mutex>

namespace events {

EventQueue::EventQueue() : event_queue(&priority_compare) {}

void EventQueue::add(Event event) {
    std::scoped_lock lock(event_queue_mutex);
    event_queue.push(event);
}

std::optional<Event> EventQueue::remove() {
    std::scoped_lock lock(event_queue_mutex);
    if (event_queue.empty()) {
        return {};
    }

    Event event = event_queue.top();
    event_queue.pop();

    return event;
}

bool EventQueue::priority_compare(const Event& a, const Event& b) {
    return a.priority < b.priority;
}

}  // namespace events
