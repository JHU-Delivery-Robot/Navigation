#include "motor_hal.hpp"

using namespace hal;

Motor::Motor(unsigned int channel) {

}

int Motor::set_speed(float speed) {

}

float Motor::get_speed(void) {

}

void Motor::reset_odometry() {

}

void Motor::set_odometry_period(float secs) {

}

OdomReading Motor::get_odometry_reading() {

}

void Motor::set_odom_callback(OdomCallback func) {

}

MotorStatus Motor::status() {

}

MotorAssembly::MotorAssembly() :
    front_l(0), front_r(1), back_l(2), back_r(3) {

}

Motor& get_motor_by_idx(unsigned int idx);

MotorAssembly robot_motors;
