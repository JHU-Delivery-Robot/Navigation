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
    std::vector<common::Vector2> waypoints;
    std::vector<common::Vector2>::iterator current_waypoint;

    Drivetrain drivetrain;
    GradientPotentialMap potential_map;
};

}  // namespace robot

#endif
