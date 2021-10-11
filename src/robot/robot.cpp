#include "robot.hpp"

#include "common.hpp"
#include "vector2.hpp"

namespace robot {

Robot::Robot(hal::HALProvider* hal)
    : hal(hal),
      drivetrain(hal->motor_assembly(), hal->gyroscope()),
      potential_map(600, 0.08, 1E5, common::Vector2(500, 500)) {
    drivetrain.setPose(common::Vector2(-300.0, -200.0), 0.0 * PI);
    potential_map.updateGoal(common::Vector2(350.0, 300.0));

    hal->motor_assembly()->reset_odometry();
}

void Robot::update() {
    drivetrain.updateOdometry();
    auto [position, heading] = drivetrain.getPose();

    auto lidar_scan = hal->lidar()->read();
    potential_map.updateLidar(lidar_scan, heading);

    common::Vector2 gradient = potential_map.getGradient(position + common::Vector2::polar(heading, 0.5 * drivetrain.length));

    if (gradient.magnitude() >= 500.0) {
        gradient = gradient * (500.0 / gradient.magnitude());
    }

    drivetrain.setCommand(gradient);
}

}  // namespace robot
