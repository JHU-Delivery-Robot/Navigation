#ifndef COMMS_HPP
#define COMMS_HPP

#include <grpc/grpc.h>
// header defines ERROR macro which is highly irritating,
// so I'm undef'ing it as close to the source as I can
#undef ERROR

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <fstream>
#include <future>
#include <memory>
#include <string>
#include <vector>

#include <google/protobuf/message.h>

#include "common/periodic.hpp"
#include "common/vector2.hpp"
#include "common/coordinates.hpp"
#include "events/error_reporting.hpp"
#include "events/route_control.hpp"
#include "hal/positioning.hpp"

#include "routing.grpc.pb.h"
#include "development.grpc.pb.h"

namespace comms {

class Comms {
public:
    Comms(std::string server_url, events::RouteControl route_control, events::ErrorReporting error_reporting, hal::Positioning *positioning);

    bool open();
    bool close();

    bool overrideRoute(std::vector<common::Coordinates> route_override);

private:
    std::shared_ptr<grpc::ChannelCredentials> constructSSLCredentials() const;
    protocols::routing::RobotStatus currentStatus() const;
    std::vector<common::Vector2> translateRoute(protocols::routing::Route route) const;

    std::string server_url;

    std::shared_ptr<grpc::ChannelInterface> channel;
    std::unique_ptr<protocols::routing::Routing::Stub> routing_stub;
    std::unique_ptr<protocols::development::Development::Stub> development_stub;

    events::ErrorReporting error_reporting;
    events::RouteControl route_control;
    hal::Positioning *positioning;

    static constexpr int poll_interval_ms = 2000;

    common::Periodic poll_task;
    void poll();
};

}  // namespace comms

#endif
