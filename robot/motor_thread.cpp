#include "motor_thread.hpp"
#include <thread>
#include <unistd.h>

MotorThread::MotorThread(MotorPositions positions, float wheelDiameter, int nAngles, int qStar,
                         int gradientScale, Vec2d goal, uint16_t *lidarData, Vec2d &pos,
                         double &head) : position(pos), heading(head) {
    potenMap = GradientPotentialMap(qStar, gradientScale, goal, lidarData);
    controller = PotentialMapSpeedController(positions, wheelDiameter, *potenMap);
    th(start_thread);
}

void MotorThread::start_thread() {
    while (1) {
        controller.updateSpeed(position, heading);
        usleep(20000); // TODO: Set this to run at regular intervals rather and add the ability to stop
    }
}
