#ifndef LOOP_HPP
#define LOOP_HPP

#include <chrono>

namespace loop {

class Loop {
protected:
    std::chrono::high_resolution_clock clock;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastLoopUpdate;
    std::chrono::milliseconds updateInterval;
    bool shouldBeRunning;
public:
    Loop(int interval);

    virtual ~RobotLoop() = 0;

    virtual void doWork() = 0;

    void looping();

    void cancel();
};

}

#endif