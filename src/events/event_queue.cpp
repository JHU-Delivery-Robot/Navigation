#include "event_queue.hpp"

#include <mutex>

namespace events {

EventQueue::EventQueue() : event_queue(&priority_compare) {}

void EventQueue::add(std::shared_ptr<Event> event) {
    if (event == nullptr) {
        return;
    }

    std::scoped_lock lock(event_queue_mutex);
    event_queue.push(event);
}

std::shared_ptr<Event> EventQueue::remove() {
    std::scoped_lock lock(event_queue_mutex);
    if (event_queue.empty()) {
        return {};
    }

    std::shared_ptr<Event> event = event_queue.top();
    event_queue.pop();

    return event;
}

bool EventQueue::priority_compare(const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b) {
    return a->getPriority() < b->getPriority();
}

}  // namespace events
