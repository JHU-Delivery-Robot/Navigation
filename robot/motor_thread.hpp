#ifndef MOTOR_THREAD_HPP
#define MOTOR_THREAD_HPP

#include "robot_thread.hpp"
#include "speed_controller.hpp"
#include "types.hpp"

class MotorThread : public RobotThread {
private:
    PotentialMapSpeedController controller;
    std::thread th;
public:
    MotorThread(MotorPositions positions, float wheelDiameter, int nAngles, int qStar, int gradientScale,
                Vec2d goal, uint16_t *lidarData);
    void start_thread() override;
};

#endif