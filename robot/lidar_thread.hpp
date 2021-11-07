#ifndef LIDAR_THREAD_HPP
#define LIDAR_THREAD_HPP

#include "robot_thread.hpp"

Class LidarThread : public RobotThread {
public:
    LidarThread();
    void start_thread();
};

#endif