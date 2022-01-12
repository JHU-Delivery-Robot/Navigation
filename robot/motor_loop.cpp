#include "motor_loop.hpp"
#include <unistd.h>

MotorLoop::MotorLoop(MotorPositions positions, float wheelDiameter, int nAngles, int qStar,
                     int gradientScale, Vec2d goal, uint16_t *lidarData, Vec2d &pos,
                     double &head, int interval) : RobotLoop(interval), position{pos}, heading{head},
                                     controller(positions, wheelDiameter, &potentialMap),
                                     potentialMap(nAngles, qStar, gradientScale, goal, lidarData) {}

MotorLoop::doWork() {
    controller.updateSpeed(position, heading);
}
