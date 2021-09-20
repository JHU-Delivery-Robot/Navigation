#include "motor_hal.hpp"

using namespace hal;

Motor::Motor(unsigned int channel) {}

int Motor::set_speed(float speed)
{
    return 0;
}

float Motor::get_speed(void)
{
    return 0.0;
}

void Motor::reset_odometry() {}

void Motor::set_odometry_period(float secs) {}

OdomReading Motor::get_odometry_reading()
{
    return 0;
}

void Motor::set_odom_callback(OdomCallback func) {}

MotorStatus Motor::status()
{
    return MotorStatus::NOMINAL;
}

MotorAssembly::MotorAssembly()
    : front_l(0), front_r(1), back_l(2), back_r(3) {}

Motor &MotorAssembly::get_motor_by_idx(unsigned int idx)
{
    return front_l;
};

MotorAssembly robot_motors;
