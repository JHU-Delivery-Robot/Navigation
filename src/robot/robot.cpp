#include "robot.hpp"

#include "common.hpp"
#include "vector2.hpp"

namespace robot {

Robot::Robot(hal::HALProvider* hal)
    : hal(hal),
      drivetrain(hal->motor_assembly(), hal->gyroscope()),
      potential_map(600, 0.08, 1E5, std::get<0>(hal->gps()->location())) {
    auto [position, heading] = hal->gps()->location();
    drivetrain.setPose(position, heading);
    hal->motor_assembly()->reset_odometry();
}

void Robot::updateGoal(common::Vector2 goal) {
    this->potential_map.updateGoal(goal);
}

void Robot::update() {
    drivetrain.updateOdometry();
    auto [position, heading] = drivetrain.getPose();

    auto lidar_scan = hal->lidar()->read();
    potential_map.updateLidarScan(lidar_scan);

    common::Vector2 gradient = potential_map.getGradient(position + common::Vector2::polar(heading, 0.5 * drivetrain.length));

    if (gradient.magnitude() >= 500.0) {
        gradient = gradient * (500.0 / gradient.magnitude());
    }

    drivetrain.setCommand(gradient);
}

}  // namespace robot
