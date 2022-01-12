#include "motor_loop.hpp"
#include <unistd.h>

MotorLoop::MotorLoop(MotorPositions positions, float wheelDiameter, int nAngles, int qStar,
                     int gradientScale, Vec2d goal, uint16_t *lidarData, Vec2d &pos,
                     double &head) : position(pos), heading(head) {
    potentialMap = GradientPotentialMap(qStar, gradientScale, goal, lidarData);
    controller = PotentialMapSpeedController(positions, wheelDiameter, *potenMap);
}

MotorLoop::loop() {
    while (1) {
        controller.updateSpeed(position, heading);
        usleep(20000); // TODO: Set this to run at regular intervals rather and add the ability to stop
    }
}
