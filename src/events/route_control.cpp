#include "route_control.hpp"

#include "route_update_event.hpp"

namespace events {

RouteControl::RouteControl(EventQueue* event_queue) : event_queue(event_queue) {}

void RouteControl::updateRoute(std::vector<common::Vector2> updated_route) {
    std::shared_ptr<Event> update = std::make_shared<RouteUpdateEvent>(updated_route, Event::LOW_PRIORITY);
    event_queue->add(update);
}

}  // namespace events
