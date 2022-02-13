#include "error_event.hpp"

namespace events {

ErrorEvent::ErrorEvent(std::string origin, std::string details, int priority)
    : Event(priority, Event::Type::FATAL_ERROR), origin(origin), details(details) {}

std::string ErrorEvent::getOrigin() const {
    return origin;
}

std::string ErrorEvent::getDetails() const {
    return details;
}

}  // namespace events
