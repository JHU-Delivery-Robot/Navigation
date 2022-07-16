#include "comms.hpp"

#include <grpcpp/grpcpp.h>

#include <functional>

#include "common/vector2.hpp"

namespace comms {

Comms::Credentials::Credentials(std::string root_ca_cert, std::string robot_cert, std::string robot_key) : root_ca_cert(root_ca_cert), robot_cert(robot_cert), robot_key(robot_key) {}

Comms::Comms(std::string server_url, Comms::Credentials credentials, events::RouteControl route_control, events::ErrorReporting error_reporting, hal::Positioning* positioning)
    : server_url(server_url),
      credentials(credentials),
      error_reporting(error_reporting),
      route_control(route_control),
      positioning(positioning),
      poll_task("comms", poll_interval_ms, std::bind(&Comms::poll, this)) {}

bool Comms::open() {
    auto credentials = constructSSLCredentials();
    if (credentials == nullptr) {
        return false;
    }

    channel = grpc::CreateChannel(server_url, credentials);
    if (channel->GetState(false) == grpc_connectivity_state::GRPC_CHANNEL_SHUTDOWN) {
        return false;
    }

    routing_stub = protocols::routing::Routing::NewStub(channel);
    development_stub = protocols::development::Development::NewStub(channel);

    poll_task.start();

    return true;
}

bool Comms::close() {
    poll_task.stop();

    return true;
}

std::shared_ptr<grpc::ChannelCredentials> Comms::constructSSLCredentials() const {
    std::ifstream ca_cert_ifs(credentials.root_ca_cert);
    if (!ca_cert_ifs.is_open()) {
        std::cout << "Failed to open CA cert" << std::endl;
        return nullptr;
    }

    std::string ca_cert((std::istreambuf_iterator<char>(ca_cert_ifs)),
                        (std::istreambuf_iterator<char>()));

    std::ifstream robot_cert_ifs(credentials.robot_cert);
    if (!robot_cert_ifs.is_open()) {
        std::cout << "Failed to open robot cert" << std::endl;
        return nullptr;
    }

    std::string robot_cert((std::istreambuf_iterator<char>(robot_cert_ifs)),
                           (std::istreambuf_iterator<char>()));

    std::ifstream robot_key_ifs(credentials.robot_key);
    if (!robot_key_ifs.is_open()) {
        std::cout << "Failed to open robot key" << std::endl;
        return nullptr;
    }

    std::string robot_key(std::istreambuf_iterator<char>(robot_key_ifs), (std::istreambuf_iterator<char>()));

    grpc::SslCredentialsOptions ssl_opts;
    ssl_opts.pem_root_certs = ca_cert;
    ssl_opts.pem_private_key = robot_key;
    ssl_opts.pem_cert_chain = robot_cert;

    return grpc::SslCredentials(ssl_opts);
}
bool Comms::overrideRoute(std::vector<common::Coordinates> route_override) {
    grpc::ClientContext context;

    protocols::routing::Route route;
    for (auto& coordinates : route_override) {
        protocols::routing::Point* waypoint = route.add_waypoints();
        waypoint->set_latitude(coordinates.latitude);
        waypoint->set_longitude(coordinates.longitude);
    }

    protocols::development::RouteResponse response;
    grpc::Status grpc_status;

    try {
        grpc_status = development_stub->SetRoute(&context, route, &response);
    } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    } catch (const std::string& ex) {
        std::cout << ex << std::endl;
    } catch (...) {
        std::cout << "Uhhhhh" << std::endl;
    }

    if (!grpc_status.ok()) {
        std::cout << "Comms error: " << std::to_string(grpc_status.error_code()) << ": " << grpc_status.error_message() << std::endl;
    }

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
