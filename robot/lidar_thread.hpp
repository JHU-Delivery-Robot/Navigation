#ifndef LIDAR_THREAD_HPP
#define LIDAR_THREAD_HPP

#include "robot_thread.hpp"

class LidarThread : public RobotThread {
private:
    // TODO: Add Instance Variables to LidarThread as needed
    std::thread th;
public:
    LidarThread();
    void start_thread() override;
};

#endif