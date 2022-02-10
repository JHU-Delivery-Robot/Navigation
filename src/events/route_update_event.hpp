#ifndef ROUTE_UPDATE_EVENT_HPP
#define ROUTE_UPDATE_EVENT_HPP

#include <vector>

#include "event.hpp"
#include "vector2.hpp"

namespace events {

class RouteUpdateEvent : public Event {
public:
    RouteUpdateEvent(std::vector<common::Vector2> route);

private:
    std::vector<common::Vector2> route;
};

}  // namespace events

#endif
