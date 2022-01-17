#include "robot_loop.hpp"
#include <unistd.h>

RobotLoop::RobotLoop(hal::HALProvider* hal) : rob(hal) {}

RobotLoop::doWork() {
    rob.update();
}
