#ifndef PERIODIC_HPP
#define PERIODIC_HPP

#include <atomic>
#include <chrono>
#include <functional>
#include <future>

namespace common {

/**
 Periodically runs a given task in a separate thread, at a specified interval.

 *Not* thread-safe, do not start/stop from multiple threads without appropriate mutexes.
 */
class Periodic {
public:
    /**
      Constructs Periodic to run task every interval_ms milliseconds.

      NOTE: doesn't start running until start() is called
     */
    Periodic(int interval_ms, std::function<void()> task);

    /**
     Starts running task at specified intervals in a new thread.
     Can be called again after stop() is called.
     */
    void start();

    /**
     Stops periodic execution of task, until start() is called again.
     NOTE: may block until iteration completes.
     */
    void stop();

    /**
      Returns true if Periodic is currently running.
     */
    bool isRunning();

private:
    std::function<void()> task;
    std::future<void> executor;
    std::atomic<bool> running;

    std::chrono::high_resolution_clock clock;
    std::chrono::milliseconds interval;
};

}  // namespace common

#endif
