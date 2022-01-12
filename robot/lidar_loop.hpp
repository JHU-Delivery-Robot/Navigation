#ifndef LIDAR_LOOP_HPP
#define LIDAR_LOOP_HPP

#include "robot_loop.hpp"

class LidarLoop : public RobotLoop {
public:
    LidarLoop();
    void loop() override;
};

#endif