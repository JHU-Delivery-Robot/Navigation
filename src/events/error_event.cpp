#include "error_event.hpp"

namespace events {

ErrorEvent::ErrorEvent(std::string origin, std::string details, int priority)
    : Event(priority), origin(origin), details(details) {}

}  // namespace events
