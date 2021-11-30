#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <vector>

#include "drivetrain.hpp"
#include "potential_map.hpp"

namespace robot {

class Robot {
public:
    Robot(hal::HALProvider* hal);

    void setWaypoints(std::vector<common::Vector2> waypoints);
    void update();

private:
    hal::HALProvider* hal;

    static constexpr double waypoint_transition_threshold = 200;
    static constexpr double q_star = 600;
    static constexpr double potential_attractive_coefficient = 0.5;
    static constexpr double potential_repulsive_coefficient = 1E5;

    std::vector<common::Vector2> waypoints;
    std::vector<common::Vector2>::iterator current_waypoint;

    Drivetrain drivetrain;
    PotentialMap potential_map;
};

}  // namespace robot

#endif
