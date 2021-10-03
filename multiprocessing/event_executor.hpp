#ifndef EVENT_EXECUTOR_HPP
#define EVENT_EXECUTOR_HPP

#include "runnable.hpp"

#include <chrono>

class EventExecutor {
public:
    virtual ~EventExecutor() {};

    /**
    * Register a loop that will start executing as soon as the
    * Executor is started. Each loop is guaranteed to be concurrent.
    */
    virtual void registerLoop(Runnable &loop, void *data) = 0;
    /**
    * Register an event to fire on an interval. Note that this may consume
    * extra concurrency units, so be careful with use.
    */
    virtual void registerTimedEvent(std::chrono::milliseconds interval, Event event) = 0;
    /**
    * Convenience function to register a handler that is executed
    * regularly at a defined interval
    * @param interval time between executions
    * @param handler handling code to run once the time is up
    */
    virtual void registerTimedHandler(std::chrono::milliseconds interval, Runnable &handler) = 0;
    /**
    * Register a handler that fires on a given Event.
    */
    virtual void registerEventHandler(Runnable &handler, Event event) = 0;
    /**
    * Start the execution loops.
    */
    virtual void start() = 0;
};

#endif