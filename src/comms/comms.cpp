#include "comms.hpp"

#include <grpcpp/grpcpp.h>

#include "common/vector2.hpp"

namespace comms {

Comms::Comms(std::string server_url, events::RouteControl route_control, hal::Positioning* positioning)
    : channel(grpc::CreateChannel(server_url, grpc::InsecureChannelCredentials())),
      stub(routing::Routing::NewStub(channel)),
      route_control(route_control),
      positioning(positioning),
      comms_open(false) {}

bool Comms::open() {
    comms_open = true;
    comms_executor = std::async(std::launch::async, [&]() {
        poll();
    });

    return true;
}

bool Comms::close() {
    comms_open = false;
    return true;
}

void Comms::poll() {
    while (comms_open) {
        grpc::ClientContext context;
        routing::Route route_response;
        routing::RobotStatus status;

        routing::Point location;
        location.set_latitude(39.325702);
        location.set_longitude(-76.621478);

        status.set_status(routing::RobotStatus::OK);
        status.mutable_location()->CopyFrom(location);

        stub->GetRoute(&context, status, &route_response);

        std::cout << "Route: ";

        std::vector<common::Vector2> updated_route = std::vector<common::Vector2>();
        for (int i = 0; i < route_response.waypoints_size(); i++) {
            const routing::Point& waypoint_coordinates = route_response.waypoints(i);
            const common::Vector2 waypoint = common::Vector2(waypoint_coordinates.longitude(), waypoint_coordinates.latitude());
            std::cout << waypoint;

            if (i < route_response.waypoints_size() - 1) {
                std::cout << ", ";
            }

            updated_route.push_back(waypoint);
        }

        std::cout << std::endl;

        route_control.updateRoute(updated_route);
    }
}

}  // namespace comms
