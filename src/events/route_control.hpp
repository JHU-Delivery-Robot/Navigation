#ifndef ROUTE_CONTROL_HPP
#define ROUTE_CONTROL_HPP

#include <memory>
#include <vector>

#include "event_queue.hpp"
#include "vector2.hpp"

namespace events {

class RouteControl {
public:
    RouteControl(EventQueue* event_queue);

    void updateRoute(std::vector<common::Vector2> updated_route);

private:
    EventQueue* event_queue;
};

}  // namespace events

#endif
