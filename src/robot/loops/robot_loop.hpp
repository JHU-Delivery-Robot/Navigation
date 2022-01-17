#ifndef MOTOR_LOOP_HPP
#define MOTOR_LOOP_HPP

#include "types.hpp"
#include "potential_map.hpp"
#include "speed_controller.hpp"
#include "loop.hpp"
#include "robot.hpp"

class RobotLoop : public Loop {
private:
    robot::Robot rob;
public:
    RobotLoop(hal::HALProvider* hal);
    void doWork() override;
};

#endif