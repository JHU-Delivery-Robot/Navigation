#include "motor_thread.hpp"
#include <thread>

MotorThread::MotorThread(MotorPositions positions, float wheelDiameter, int nAngles, int qStar, int gradientScale,
                         Vec2d goal, uint16_t *lidarData) {
    // Initialize Data Here
    th(start_thread);
}

void MotorThread::start_thread() {
    // Implement Function to run in thread
}
