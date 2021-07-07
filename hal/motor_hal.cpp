#include "motor_hal.hpp"

using namespace hal;

Motor::Motor(unsigned int channel, Coord R, Angle Theta) :
 r(R), theta(Theta) {

}

int Motor::set_speed(float speed) {

}

float Motor::get_speed(void) {

}

Coord get_r() {
    return 0;
}

Angle get_theta() {
    return 0;
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
    front_l(0, 0, 0), front_r(0, 0, 0), back_l(0, 0, 0), back_r(0, 0, 0) {

}

Motor& get_motor_by_idx(unsigned int idx);

MotorAssembly robot_motors;
