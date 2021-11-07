#ifndef MOTOR_THREAD_HPP
#define MOTOR_THREAD_HPP

#include "robot_thread.hpp"

Class MotorThread : public RobotThread {
public:
    MotorThread();
    void start_thread();
};

#endif