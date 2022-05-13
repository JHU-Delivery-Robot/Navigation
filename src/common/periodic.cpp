#include "periodic.hpp"

#include <iostream>
#include <thread>

namespace common {

Periodic::Periodic(int interval_ms, std::function<void()> task)
    : task(task), running(false), interval(interval_ms) {}

void Periodic::start() {
    if (running) {
        return;
    }

    running = true;

    executor = std::async(std::launch::async, [&]() {
        auto next_execution = clock.now();

        while (running) {
            task();

            next_execution = next_execution += interval;
            if (clock.now() > next_execution) {
                std::chrono::milliseconds overrun = std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - next_execution);
                next_execution = clock.now();
                std::cout << "Loop over-run by " << overrun.count() << " ms" << std::endl;
                continue;
            }

            std::this_thread::sleep_until(next_execution);
        }
    });
}

void Periodic::stop() {
    running = false;

    // If we ever use this with intervals longer than a few seconds
    // or where start/stop are used during normal operation, we may
    // want to use a condition variable to cancel the sleep early.
    executor.wait();
}

bool Periodic::isRunning() {
    return running;
}

}  // namespace common
