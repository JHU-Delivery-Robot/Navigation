#include "error_event.hpp"

namespace events {

ErrorEvent::ErrorEvent(std::string origin, std::string details) : Event(0), origin(origin), details(details) {}

}  // namespace events
