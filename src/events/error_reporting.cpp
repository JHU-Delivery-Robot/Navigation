#include "error_reporting.hpp"

#include "error_event.hpp"

namespace events {

ErrorReporting::ErrorReporting(EventQueue* event_queue) : event_queue(event_queue) {}

void ErrorReporting::reportError(std::string origin, std::string details) {
    std::shared_ptr<Event> error = std::make_shared<ErrorEvent>(origin, details, Event::HIGH_PRIORITY);
    event_queue->add(error);
}

}  // namespace events
