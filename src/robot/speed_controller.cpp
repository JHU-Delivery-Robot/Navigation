#include "speed_controller.hpp"

#include <cmath>
#include <iostream>

#include "common.hpp"
#include "potential_map.hpp"
#include "vector2.hpp"

namespace robot {

WheelPosition::WheelPosition(double theta, double radius) : theta(theta), radius(radius) {}

WheelPositions::WheelPositions(WheelPosition front_l, WheelPosition front_r, WheelPosition back_l, WheelPosition back_r)
    : front_l(front_l),
      front_r(front_r),
      back_l(back_l),
      back_r(back_r) {}

SpeedController::SpeedController(
    WheelPositions wheel_positions, double wheel_diameter, double max_speed, hal::HALProvider* hal)
    : wheel_positions(wheel_positions),
      wheel_diameter(wheel_diameter),
      max_speed(max_speed),
      motors(hal->motor_assembly()),
      positioning_system(hal->positioning()) {}

double SpeedController::angularToWheelFrequency(double ang_vel, WheelPosition position) {
    double wheel_circumference = wheel_diameter * PI;
    double ground_speed = ang_vel * position.radius * cos(position.theta);

    return ground_speed / wheel_circumference;
}

void SpeedController::updateSpeed(common::Vector2 gradient) {
    auto [position, heading] = positioning_system->getPose();

    if (gradient.magnitude() >= max_speed) {
        gradient = gradient * (max_speed / gradient.magnitude());
    }

    double speed = 100*gradient.dot(common::Vector2::polar(heading, 1.0));               // radial component of gradient
    double ang_vel = gradient.dot(common::Vector2::polar(heading + 0.5 * PI, 1.0));  // tangential component of gradient

    motors->front_left()->set_speed(speed + angularToWheelFrequency(ang_vel, wheel_positions.front_l));
    motors->front_right()->set_speed(speed + angularToWheelFrequency(ang_vel, wheel_positions.front_r));
    motors->back_left()->set_speed(speed + angularToWheelFrequency(ang_vel, wheel_positions.back_l));
    motors->back_right()->set_speed(speed + angularToWheelFrequency(ang_vel, wheel_positions.back_r));
}

}  // namespace robot
