/*
Loops required:
1. GPS, Encoders, IMU
2. Kalman (Localization)
3. Lidar
4. Robot Motors (Potential Map + Speed Controller)
5. Ultrasonic, Infrared

2 requests data from 1
4 requests data from 3, 2, and 5
*/

#ifndef LOOP_HPP
#define LOOP_HPP

#include <chrono>
#include <thread>
#include <functional>

namespace robot {

class Loop {
private:
    std::chrono::high_resolution_clock clock;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastLoopUpdate;
    std::chrono::milliseconds updateInterval;
    bool shouldBeRunning;
    std::function<void()> function;
public:
    Loop(int interval, std::function<void()> function);

    void looping();

    void cancel();
};

}

#endif