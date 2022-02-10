#include "route_update_event.hpp"

namespace events {

RouteUpdateEvent::RouteUpdateEvent(std::vector<common::Vector2> route, int priority)
    : Event(priority), route(route){};

}  // namespace events
