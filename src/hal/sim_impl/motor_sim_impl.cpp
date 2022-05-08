#include "motor_sim_impl.hpp"

namespace sim {

MotorSimImpl::MotorSimImpl() : speed(0.0), total_distance(0.0) { }

void MotorSimImpl::update(double time_delta) {
    double distance_traveled = time_delta * speed;
    total_distance += distance_traveled;
}

hal::Motor::Status MotorSimImpl::status() const {
    return hal::Motor::Status::NOMINAL;
}

void MotorSimImpl::set_speed(double speed) {
    this->speed = speed;
}

double MotorSimImpl::get_speed() const {
    return speed;
}

void MotorSimImpl::reset_odometry() {
    total_distance = 0.0;
}

double MotorSimImpl::odometry_distance() const {
    return total_distance;
}

}
