#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "hal_provider.hpp"
#include "drivetrain.hpp"
#include "potential_map.hpp"

namespace robot {

class Robot {
public:
    Robot(hal::HALProvider* hal);

    void updateGoal(common::Vector2 goal);
    void update();

private:
    hal::HALProvider* hal;

    Drivetrain drivetrain;
    GradientPotentialMap potential_map;
};

}  // namespace robot

#endif
