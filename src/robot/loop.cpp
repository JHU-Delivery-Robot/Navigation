#include "loop.hpp"

namespace robot {

Loop::Loop(int interval, std::function<void()> function)
    : updateInterval(interval), shouldBeRunning(true), function(function) {}

void Loop::cancel() {
    shouldBeRunning = false;
}

void Loop::looping() {
    while (shouldBeRunning) {
        lastLoopUpdate = clock.now();
        function();
        std::chrono::duration duration = clock.now() - lastLoopUpdate;
        std::this_thread::sleep_for(updateInterval - duration);
    }
}

}