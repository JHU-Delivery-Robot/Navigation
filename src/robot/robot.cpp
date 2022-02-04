#include "robot.hpp"

#include <iostream>
#include <iterator>

#include "common.hpp"
#include "vector2.hpp"

namespace robot {

Robot::Robot(hal::HALProvider* hal)
    : hal(hal),
      drivetrain(hal->motor_assembly(), hal->gyroscope()),
      potential_map(600, 0.5, 1E5, std::get<0>(hal->gps()->location())) {
    auto [position, heading] = hal->gps()->location();
    drivetrain.setPose(position, heading);
    hal->motor_assembly()->reset_odometry();
}

void Robot::setWaypoints(std::vector<common::Vector2> waypoints) {
    this->waypoints = waypoints;
    current_waypoint = this->waypoints.begin();
    potential_map.updateGoal(*current_waypoint);
}

void Robot::update() {
    drivetrain.updateOdometry();
    auto [position, heading] = drivetrain.getPose();

    auto lidar_scan = hal->lidar()->getLatestScan();
    potential_map.updateLidarScan(lidar_scan);

    // Once we are close enough to the current waypoint, set goal to the next waypoint
    if ((*current_waypoint - position).magnitude() < waypoint_transition_threshold) {
        auto next_waypoint = std::next(current_waypoint);
        if (next_waypoint != waypoints.end()) {
            current_waypoint = next_waypoint;
            potential_map.updateGoal(*current_waypoint);
        }
    }

    common::Vector2 gradient = potential_map.getGradient(position + common::Vector2::polar(heading, 0.5 * drivetrain.length));

    if (gradient.magnitude() >= 200.0) {
        gradient = gradient * (200.0 / gradient.magnitude());
    }

    drivetrain.setCommand(gradient);
}

}  // namespace robot
