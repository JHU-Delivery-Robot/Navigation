#include "route_update_event.hpp"

namespace events {

RouteUpdateEvent::RouteUpdateEvent(std::vector<common::Vector2> route) : Event(1), route(route){};

}  // namespace events
