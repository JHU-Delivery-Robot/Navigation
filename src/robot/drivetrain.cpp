#include "drivetrain.hpp"

#include <algorithm>
#include <cmath>

namespace robot {

Drivetrain::Drivetrain(hal::HALProvider::MotorAssembly* motors, hal::Gyroscope* gyroscope)
    : odometry(wheel_base_width), motors(motors), gyroscope(gyroscope) {}

void Drivetrain::setCommand(common::Vector2 drive_command) {
    double l = 0.5 * length;
    double w = 0.5 * wheel_base_width;

    auto [position, heading] = odometry.getPose();
    double parallel_component = drive_command.dot(common::Vector2::polar(heading, 1.0));
    double orthogonal_component = drive_command.dot(common::Vector2::polar(heading - 0.5 * PI, 1.0));

    double left_speed, right_speed;

    parallel_component = std::max(parallel_component, 0.0);

    if (std::abs(orthogonal_component) < 1e-1) {
        left_speed = parallel_component;
        right_speed = parallel_component;
    } else {
        double robot_path_radius = -l * (parallel_component / orthogonal_component);
        double front_path_radius = std::copysign(std::sqrt(l * l + robot_path_radius * robot_path_radius), -orthogonal_component);

        double left_radius = robot_path_radius - w;
        double right_radius = robot_path_radius + w;

        double speed = drive_command.magnitude();

        left_speed = (left_radius / front_path_radius) * speed;
        right_speed = (right_radius / front_path_radius) * speed;
    }

    motors->front_left()->set_speed(left_speed);
    motors->back_left()->set_speed(left_speed);

    motors->front_right()->set_speed(right_speed);
    motors->back_right()->set_speed(right_speed);
}

void Drivetrain::setPose(common::Vector2 position, double heading) {
    odometry.setPose(position, heading);
}

std::tuple<common::Vector2, double> Drivetrain::getPose() {
    return odometry.getPose();
}

void Drivetrain::updateOdometry() {
    double new_left_distance = 0.5 * (motors->front_left()->odometry_distance() + motors->back_left()->odometry_distance());
    double new_right_distance = 0.5 * (motors->front_right()->odometry_distance() + motors->back_right()->odometry_distance());
    double new_heading = gyroscope->angle();

    odometry.updateOdometry(new_heading, new_left_distance, new_right_distance);
}

}  // namespace robot
