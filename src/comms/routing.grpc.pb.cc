// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: routing.proto

#include "routing.pb.h"
#include "routing.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace protocols {
namespace routing {

static const char* Routing_method_names[] = {
  "/protocols.routing.Routing/GetRoute",
};

std::unique_ptr< Routing::Stub> Routing::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Routing::Stub> stub(new Routing::Stub(channel, options));
  return stub;
}

Routing::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetRoute_(Routing_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Routing::Stub::GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::protocols::routing::Route* response) {
  return ::grpc::internal::BlockingUnaryCall< ::protocols::routing::RobotStatus, ::protocols::routing::Route, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetRoute_, context, request, response);
}

void Routing::Stub::async::GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::protocols::routing::RobotStatus, ::protocols::routing::Route, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetRoute_, context, request, response, std::move(f));
}

void Routing::Stub::async::GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetRoute_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::protocols::routing::Route>* Routing::Stub::PrepareAsyncGetRouteRaw(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::protocols::routing::Route, ::protocols::routing::RobotStatus, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetRoute_, context, request);
}

::grpc::ClientAsyncResponseReader< ::protocols::routing::Route>* Routing::Stub::AsyncGetRouteRaw(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetRouteRaw(context, request, cq);
  result->StartCall();
  return result;
}

Routing::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Routing_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Routing::Service, ::protocols::routing::RobotStatus, ::protocols::routing::Route, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Routing::Service* service,
             ::grpc::ServerContext* ctx,
             const ::protocols::routing::RobotStatus* req,
             ::protocols::routing::Route* resp) {
               return service->GetRoute(ctx, req, resp);
             }, this)));
}

Routing::Service::~Service() {
}

::grpc::Status Routing::Service::GetRoute(::grpc::ServerContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace protocols
}  // namespace routing

