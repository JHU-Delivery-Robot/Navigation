// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: routing.proto
#ifndef GRPC_routing_2eproto__INCLUDED
#define GRPC_routing_2eproto__INCLUDED

#include "routing.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace protocols {
namespace routing {

class Routing final {
 public:
  static constexpr char const* service_full_name() {
    return "protocols.routing.Routing";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::protocols::routing::Route* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::protocols::routing::Route>> AsyncGetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::protocols::routing::Route>>(AsyncGetRouteRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::protocols::routing::Route>> PrepareAsyncGetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::protocols::routing::Route>>(PrepareAsyncGetRouteRaw(context, request, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      virtual void GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response, std::function<void(::grpc::Status)>) = 0;
      virtual void GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response, ::grpc::ClientUnaryReactor* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::protocols::routing::Route>* AsyncGetRouteRaw(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::protocols::routing::Route>* PrepareAsyncGetRouteRaw(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::protocols::routing::Route* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::protocols::routing::Route>> AsyncGetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::protocols::routing::Route>>(AsyncGetRouteRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::protocols::routing::Route>> PrepareAsyncGetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::protocols::routing::Route>>(PrepareAsyncGetRouteRaw(context, request, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response, std::function<void(::grpc::Status)>) override;
      void GetRoute(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response, ::grpc::ClientUnaryReactor* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::protocols::routing::Route>* AsyncGetRouteRaw(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::protocols::routing::Route>* PrepareAsyncGetRouteRaw(::grpc::ClientContext* context, const ::protocols::routing::RobotStatus& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_GetRoute_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status GetRoute(::grpc::ServerContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_GetRoute : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_GetRoute() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_GetRoute() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetRoute(::grpc::ServerContext* /*context*/, const ::protocols::routing::RobotStatus* /*request*/, ::protocols::routing::Route* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetRoute(::grpc::ServerContext* context, ::protocols::routing::RobotStatus* request, ::grpc::ServerAsyncResponseWriter< ::protocols::routing::Route>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_GetRoute<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_GetRoute : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_GetRoute() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::protocols::routing::RobotStatus, ::protocols::routing::Route>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::protocols::routing::RobotStatus* request, ::protocols::routing::Route* response) { return this->GetRoute(context, request, response); }));}
    void SetMessageAllocatorFor_GetRoute(
        ::grpc::MessageAllocator< ::protocols::routing::RobotStatus, ::protocols::routing::Route>* allocator) {
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
      static_cast<::grpc::internal::CallbackUnaryHandler< ::protocols::routing::RobotStatus, ::protocols::routing::Route>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~WithCallbackMethod_GetRoute() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetRoute(::grpc::ServerContext* /*context*/, const ::protocols::routing::RobotStatus* /*request*/, ::protocols::routing::Route* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* GetRoute(
      ::grpc::CallbackServerContext* /*context*/, const ::protocols::routing::RobotStatus* /*request*/, ::protocols::routing::Route* /*response*/)  { return nullptr; }
  };
  typedef WithCallbackMethod_GetRoute<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_GetRoute : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_GetRoute() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_GetRoute() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetRoute(::grpc::ServerContext* /*context*/, const ::protocols::routing::RobotStatus* /*request*/, ::protocols::routing::Route* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_GetRoute : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_GetRoute() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_GetRoute() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetRoute(::grpc::ServerContext* /*context*/, const ::protocols::routing::RobotStatus* /*request*/, ::protocols::routing::Route* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetRoute(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_GetRoute : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_GetRoute() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->GetRoute(context, request, response); }));
    }
    ~WithRawCallbackMethod_GetRoute() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetRoute(::grpc::ServerContext* /*context*/, const ::protocols::routing::RobotStatus* /*request*/, ::protocols::routing::Route* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerUnaryReactor* GetRoute(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)  { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_GetRoute : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_GetRoute() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::protocols::routing::RobotStatus, ::protocols::routing::Route>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::protocols::routing::RobotStatus, ::protocols::routing::Route>* streamer) {
                       return this->StreamedGetRoute(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_GetRoute() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status GetRoute(::grpc::ServerContext* /*context*/, const ::protocols::routing::RobotStatus* /*request*/, ::protocols::routing::Route* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGetRoute(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::protocols::routing::RobotStatus,::protocols::routing::Route>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_GetRoute<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_GetRoute<Service > StreamedService;
};

}  // namespace routing
}  // namespace protocols


#endif  // GRPC_routing_2eproto__INCLUDED
