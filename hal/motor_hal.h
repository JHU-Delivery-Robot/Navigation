#ifndef _MOTOR_HAL_H_
#define _MOTOR_HAL_H_

#include <pthread.h>

#include "hal_types.h"

namespace hal {

enum MotorStatus {
    NOMINAL = 0,
    LOCKED,
    HOT
};

class Motor {
private:
    pthread_mutex_t lock_motor;
public:
    Motor(unsigned int channel);
    int set_speed(float speed);
    float get_speed();
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
