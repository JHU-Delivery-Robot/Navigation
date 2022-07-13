#include "comms.hpp"

#include <grpcpp/grpcpp.h>

#include <functional>

#include "common/vector2.hpp"

namespace comms {

Comms::Comms(std::string server_url, events::RouteControl route_control, events::ErrorReporting error_reporting, hal::Positioning* positioning)
    : channel(grpc::CreateChannel(server_url, grpc::InsecureChannelCredentials())),
      routing_stub(protocols::routing::Routing::NewStub(channel)),
      development_stub(protocols::development::Development::NewStub(channel)),
      error_reporting(error_reporting),
      route_control(route_control),
      positioning(positioning),
      poll_task("comms", poll_interval_ms, std::bind(&Comms::poll, this)) {}

bool Comms::open() {
    poll_task.start();

    return true;
}

bool Comms::close() {
    poll_task.stop();

    return true;
}

bool Comms::overrideRoute(std::vector<common::Coordinates> route_override) const {
    grpc::ClientContext context;

    protocols::routing::Route route;
    for (auto& coordinates : route_override) {
        protocols::routing::Point* waypoint = route.add_waypoints();
        waypoint->set_latitude(coordinates.latitude);
        waypoint->set_longitude(coordinates.longitude);
    }

    protocols::development::RouteResponse response;
    grpc::Status grpc_status = development_stub->SetRoute(&context, route, &response);

    return grpc_status.ok();
}

void Comms::poll() {
    grpc::ClientContext context;
    
    protocols::routing::Route route;
    grpc::Status grpc_status = routing_stub->GetRoute(&context, currentStatus(), &route);

    if (!grpc_status.ok()) {
        if (grpc_status.error_code() == grpc::StatusCode::UNAVAILABLE) {
            // Sometimes robot's WiFi signal is poor, it is ok if server is unavailable for a while.
            return;
        } else {
            error_reporting.reportError("Comms", std::to_string(grpc_status.error_code()) + ": " + grpc_status.error_message());
        }

        return;
    }

    std::vector<common::Vector2> updated_route = translateRoute(route);
    route_control.updateRoute(updated_route);
}

protocols::routing::RobotStatus Comms::currentStatus() const {
    auto [position, orientation] = positioning->getPose();
    common::Coordinates current_location = positioning->coordinateSystem().project(position);

    protocols::routing::Point location;
    location.set_latitude(current_location.latitude);
    location.set_longitude(current_location.longitude);

    protocols::routing::RobotStatus status;
    status.set_status(protocols::routing::RobotStatus::OK);
    status.mutable_location()->CopyFrom(location);

    return status;
}

std::vector<common::Vector2> Comms::translateRoute(protocols::routing::Route route) const {
    std::vector<common::Vector2> waypoints = std::vector<common::Vector2>();

    for (int i = 0; i < route.waypoints_size(); i++) {
        const protocols::routing::Point& waypoint = route.waypoints(i);
        const common::Coordinates coordinates = common::Coordinates(waypoint.latitude(), waypoint.longitude());
        const common::Vector2 waypoint_position = positioning->coordinateSystem().project(coordinates);
        waypoints.push_back(waypoint_position);
    }

    return waypoints;
}

}  // namespace comms
