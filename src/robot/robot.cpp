#include "robot.hpp"

#include <cmath>
#include <iterator>

#include "common.hpp"
#include "vector2.hpp"

namespace robot {

Robot::Robot(hal::HALProvider* hal)
    : hal(hal),
      potential_map(q_star, potential_attractive_coefficient, potential_repulsive_coefficient, waypoint_transition_threshold),
      speed_controller(getWheelPositions(), 2 * wheel_radius, 2.0, hal) {
    hal->motor_assembly()->reset_odometry();
}

void Robot::setWaypoints(std::vector<common::Vector2> waypoints) {
    this->waypoints = waypoints;
    current_waypoint = this->waypoints.begin();
    potential_map.updateGoal(*current_waypoint);
}

void Robot::update() {
    auto lidar_scan = hal->lidar()->getLatestScan();
    potential_map.updateLidarScan(lidar_scan);

    auto [position, heading] = hal->positioning()->getPose();

    // Once we are close enough to the current waypoint, set goal to the next waypoint
    if ((*current_waypoint - position).magnitude() < waypoint_transition_threshold) {
        auto next_waypoint = std::next(current_waypoint);
        if (next_waypoint != waypoints.end()) {
            current_waypoint = next_waypoint;
            potential_map.updateGoal(*current_waypoint);
        }
    }

    common::Vector2 gradient = potential_map.getGradient(position + common::Vector2::polar(heading, 0.5 * length));

    speed_controller.updateSpeed(gradient);
}

WheelPositions Robot::getWheelPositions() {
    double radius = std::sqrt(0.25 * length * length + 0.25 * wheel_base_width * wheel_base_width);
    double theta = std::atan2(0.5 * length, 0.5 * wheel_base_width);

    WheelPosition front_left(PI - theta, radius);
    WheelPosition front_right(theta, radius);
    WheelPosition back_left(PI + theta, radius);
    WheelPosition back_right(2 * PI - theta, radius);

    return WheelPositions(front_left, front_right, back_left, back_right);
}

}  // namespace robot
