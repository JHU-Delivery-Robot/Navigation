#include "route_update_event.hpp"

namespace events {

RouteUpdateEvent::RouteUpdateEvent(std::vector<common::Vector2> route, int priority)
    : Event(priority, Event::Type::ROUTE_UPDATE), route(route) {}

std::vector<common::Vector2> RouteUpdateEvent::getRoute() const {
    return route;
}

}  // namespace events
