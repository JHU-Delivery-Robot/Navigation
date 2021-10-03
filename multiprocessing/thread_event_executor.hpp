#ifndef THREAD_EVENT_EXECUTOR_HPP
#define THREAD_EVENT_EXECUTOR_HPP

//#include "runnable.hpp"

#include "event_executor.hpp"

//#include <chrono>

class ThreadEventExecutor : public EventExecutor {
public:
    /**
    * Register a loop that will start executing as soon as the
    * Executor is started. Each loop is guaranteed to be concurrent.
    */
    void registerLoop(Runnable &loop, void *data) override;
    /**
    * Register an event to fire on an interval. Note that this may consume
    * extra concurrency units, so be careful with use.
    */
    void registerTimedEvent(std::chrono::duration interval, Event event) override;
    /**
    * Convenience function to register a handler that is executed
    * regularly at a defined interval
    * @param interval time between executions
    * @param handler handling code to run once the time is up
    */
    void registerTimedHandler(std::chrono::duration interval, Runnable &handler) override;
    /**
    * Register a handler that fires on a given Event.
    */
    void registerEventHandler(Runnable &handler, Event event) override;
    /**
    * Start the execution loops.
    */
    void start() override;
};

#endif