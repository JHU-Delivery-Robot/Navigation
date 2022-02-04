#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <vector>

#include "common.hpp"
#include "hal_provider.hpp"
#include "potential_map.hpp"
#include "speed_controller.hpp"

namespace robot {

class Robot {
public:
    Robot(hal::HALProvider* hal);

    void setWaypoints(std::vector<common::Vector2> waypoints);
    void update();

    // all in meters
    static constexpr double wheel_base_width = 0.20;
    static constexpr double length = 0.50;
    static constexpr double wheel_radius = 0.08;
    static constexpr double wheel_circumference = 2 * PI * wheel_radius;

private:
    hal::HALProvider* hal;

    static constexpr double waypoint_transition_threshold = 1;
    static constexpr double q_star = 6;
    static constexpr double potential_attractive_coefficient = 20.0;
    static constexpr double potential_repulsive_coefficient = 0.02;

    std::vector<common::Vector2> waypoints;
    std::vector<common::Vector2>::iterator current_waypoint;

    PotentialMap potential_map;
    SpeedController speed_controller;

    static WheelPositions getWheelPositions();
};

}  // namespace robot

#endif
