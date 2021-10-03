#include "thread_event_executor.hpp"



void ThreadEventExecutor::registerLoop(Runnable &loop, void *data) {
    //Implement this!
    //loop.run(data, ExecutionContext context);
}
void ThreadEventExecutor::registerTimedEvent(std::chrono::duration interval, Event event) {
     //Implement this!
}
void ThreadEventExecutor::registerTimedHandler(std::chrono::duration interval, Runnable &handler) {
     //Implement this!
}
void ThreadEventExecutor::registerEventHandler(Runnable &handler, Event event) {
     //Implement this!
}
void ThreadEventExecutor::start() {
     //Implement this!
}