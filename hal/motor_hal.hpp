#ifndef _MOTOR_HAL_HPP_
#define _MOTOR_HAL_HPP_

#include <pthread.h>

#include "hal_types.hpp"

namespace hal {

enum MotorStatus {
    NOMINAL = 0,
    LOCKED,
    HOT
};

class Motor {
private:
    pthread_mutex_t lock_motor;
    Coord r;
    Angle theta;
public:
    Motor(unsigned int channel, Coord R, Angle Theta);
    int set_speed(float speed);
    float get_speed();
    Coord get_r();
    Angle get_theta();
    void reset_odometry();
    void set_odometry_period(float secs);
    OdomReading get_odometry_reading();
    void set_odom_callback(OdomCallback func);
    MotorStatus status();
};

class MotorAssembly {
public:
    Motor front_l;
    Motor front_r;
    Motor back_l;
    Motor back_r;

    MotorAssembly();
    Motor& get_motor_by_idx(unsigned int idx);
};

extern MotorAssembly robot_motors;

// end of namespace hal
}

#endif
