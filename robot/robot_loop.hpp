#ifndef ROBOT_LOOP_HPP
#define ROBOT_LOOP_HPP

#include <chrono>

class RobotLoop {
protected:
    std::chrono::high_resolution_clock clock;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastLoopUpdate;
    std::chrono::milliseconds updateInterval;
    bool shouldBeRunning;
public:
    RobotLoop(int interval);
    virtual ~RobotLoop() = 0;
    virtual void doWork() = 0;
    void loop();
    void cancel();
};

#endif