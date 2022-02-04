#include "differential_odometry.hpp"

#include <cmath>
#include <iostream>

#include "common.hpp"

namespace common {

DifferentialOdometry::DifferentialOdometry(double width)
    : wheel_base_width(width),
      position(0.0, 0.0),
      heading(0.0),
      left_wheels_distance(0.0),
      right_wheels_distance(0.0) {}

void DifferentialOdometry::setPose(common::Vector2 position, double heading) {
    this->position = position;
    this->heading = heading;
}

std::tuple<common::Vector2, double> DifferentialOdometry::getPose() {
    return {position, heading};
}

void DifferentialOdometry::updateOdometry(double heading, double total_left_distance, double total_right_distance) {
    double delta_left = total_left_distance - this->left_wheels_distance;
    double delta_right = total_right_distance - this->right_wheels_distance;
    double delta_angle = heading - this->heading;

    this->left_wheels_distance = total_left_distance;
    this->right_wheels_distance = total_right_distance;
    this->heading = heading;

    double distance = (delta_left + delta_right) / 2.0;

    // Keep delta_angle in range [-PI, PI]
    if (std::abs(delta_angle) > PI) {
        delta_angle = copysign(2.0 * PI - std::abs(delta_angle), -delta_angle);
    }

    if (std::abs(delta_angle) < 1e-4) {
        double delta_x = distance * std::cos(heading);
        double delta_y = distance * std::sin(heading);

        position += common::Vector2(delta_x, delta_y);

        return;
    }

    double radius = distance / delta_angle;
    double chord_angle = heading - delta_angle / 2.0;
    double chord_length = 2.0 * radius * std::sin(delta_angle / 2.0);

    double delta_x = chord_length * std::cos(chord_angle);
    double delta_y = chord_length * std::sin(chord_angle);

    position += common::Vector2(delta_x, delta_y);
}

double DifferentialOdometry::estimateRotation(double total_left_distance, double total_right_distance) const {
    double delta_left = total_left_distance - this->left_wheels_distance;
    double delta_right = total_right_distance - this->right_wheels_distance;

    double diff = delta_right - delta_left;

    double delta_theta = diff / wheel_base_width;
    return delta_theta;
}

}  // namespace common
