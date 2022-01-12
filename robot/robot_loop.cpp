#include "robot_loop.hpp"

RobotLoop::RobotLoop(int interval) : updateInterval(interval), shouldBeRunning(true) {}

RobotLoop::cancel() {
    shouldBeRunning = false;
}

RobotLoop::loop() {
    while (shouldBeRunning) {
        lastLoopUpdate = clock.now();
        doWork();
        std::chrono::duration duration = clock.now() - lastLoopUpdate;
        std::this_thread::sleep_for(updateInterval - duration);
    }
}