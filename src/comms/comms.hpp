#ifndef COMMS_HPP
#define COMMS_HPP

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>

#include <future>
#include <memory>
#include <string>

#include <google/protobuf/message.h>

#include "common/periodic.hpp"
#include "events/route_control.hpp"
#include "hal/positioning.hpp"
#include "routing.grpc.pb.h"

namespace comms {

class Comms {
public:
    Comms(std::string server_url, events::RouteControl route_control, hal::Positioning *positioning);

    bool open();
    bool close();

private:
    std::shared_ptr<grpc::ChannelInterface> channel;
    std::unique_ptr<routing::Routing::Stub> stub;
    events::RouteControl route_control;
    hal::Positioning *positioning;

    static constexpr int poll_interval_ms = 2000;

    common::Periodic poll_task;
    void poll();
};

}  // namespace comms

#endif
