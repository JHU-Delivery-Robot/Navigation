#include "loop.hpp"

Loop::Loop(int interval) : updateInterval(interval), shouldBeRunning(true) {}

Loop::cancel() {
    shouldBeRunning = false;
}

Loop::looping() {
    while (shouldBeRunning) {
        lastLoopUpdate = clock.now();
        doWork();
        std::chrono::duration duration = clock.now() - lastLoopUpdate;
        std::this_thread::sleep_for(updateInterval - duration);
    }
}