#ifndef EVENT_EXECUTOR_HPP
#define EVENT_EXECUTOR_HPP

#include "runnable.hpp"

#include <chrono>

class EventExecutor {
public:
    /**
    * Register a loop that will start executing as soon as the
    * Executor is started. Each loop is guaranteed to be concurrent.
    */
    virtual void registerLoop(Runnable &loop, void *data);
    /**
    * Register an event to fire on an interval. Note that this may consume
    * extra concurrency units, so be careful with use.
    */
    virtual void registerTimedEvent(std::chrono::duration interval, Event event);
    /**
    * Convenience function to register a handler that is executed
    * regularly at a defined interval
    * @param interval time between executions
    * @param handler handling code to run once the time is up
    */
    virtual void registerTimedHandler(TimeInterval interval, Runnable &handler);
    /**
    * Register a handler that fires on a given Event.
    */
    virtual void registerEventHandler(Runnable &handler, Event event);
    /**
    * Start the execution loops.
    */
    virtual void start() = 0;
};

#endif