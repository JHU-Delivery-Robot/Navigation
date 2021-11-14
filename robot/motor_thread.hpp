#ifndef MOTOR_THREAD_HPP
#define MOTOR_THREAD_HPP

#include "robot_thread.hpp"
#include "speed_controller.hpp"
#include "types.hpp"

class MotorThread : public RobotThread {
private:
    // TODO: Add Instance Variables to MotorThread as needed
    PotentialMapSpeedController controller;
    GradientPotentialMap potenMap;
    std::thread th;
    Vec2d& position;
    double& heading;
public:
    MotorThread(MotorPositions positions, float wheelDiameter, int nAngles, int qStar, int gradientScale,
                Vec2d goal, uint16_t *lidarData, Vec2d& position, double& heading);
    void start_thread() override;
};

#endif