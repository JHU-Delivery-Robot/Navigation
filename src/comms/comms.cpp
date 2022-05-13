#include "comms.hpp"

#include <grpcpp/grpcpp.h>

#include <functional>

#include "common/vector2.hpp"

namespace comms {

Comms::Comms(std::string server_url, events::RouteControl route_control, hal::Positioning* positioning)
    : channel(grpc::CreateChannel(server_url, grpc::InsecureChannelCredentials())),
      stub(routing::Routing::NewStub(channel)),
      route_control(route_control),
      positioning(positioning),
      poll_task(poll_interval_ms, std::bind(&Comms::poll, this)) {}

bool Comms::open() {
    poll_task.start();

    return true;
}

bool Comms::close() {
    poll_task.stop();

    return true;
}

void Comms::poll() {
    grpc::ClientContext context;
    routing::Route route_response;
    routing::RobotStatus status;

    routing::Point location;
    location.set_latitude(39.325702);
    location.set_longitude(-76.621478);

    status.set_status(routing::RobotStatus::OK);
    status.mutable_location()->CopyFrom(location);

    grpc::Status grpc_status = stub->GetRoute(&context, status, &route_response);

    if (!grpc_status.ok()) {
        // if (grpc_status.error_code() == grpc::StatusCode::UNAVAILABLE)
        std::cout << "Server comms error: " << grpc_status.error_code() << ": " << grpc_status.error_message() << std::endl;
        return;
    }

    std::vector<common::Vector2> updated_route = std::vector<common::Vector2>();
    for (int i = 0; i < route_response.waypoints_size(); i++) {
        const routing::Point& waypoint_coordinates = route_response.waypoints(i);
        const common::Vector2 waypoint = common::Vector2(waypoint_coordinates.longitude(), waypoint_coordinates.latitude());
        updated_route.push_back(waypoint);
    }

    route_control.updateRoute(updated_route);
}

}  // namespace comms
