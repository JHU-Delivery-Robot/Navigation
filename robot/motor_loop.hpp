#ifndef MOTOR_LOOP_HPP
#define MOTOR_LOOP_HPP

#include "types.hpp"
#include "potential_map.hpp"
#include "speed_controller.hpp"
#include "robot_loop.hpp"

class MotorLoop : public RobotLoop {
private:
    PotentialMapSpeedController controller;
    GradientPotentialMap potentialMap;
    Vec2d& position;
    double& heading;
public:
    MotorLoop(MotorPositions positions, float wheelDiameter, int nAngles, int qStar,
              int gradientScale, Vec2d goal, uint16_t *lidarData, Vec2d &pos,
              double &head);
    void loop() override;
};

#endif