#ifndef POSITIONING_HPP
#define POSITIONING_HPP

#include <tuple>

#include "common/vector2.hpp"

namespace hal {

class Positioning {
public:
    virtual ~Positioning() {}

    /**
     * Get the current pose of the robot,
     * as position in meters and angle in radians.
     * Angle is anti-clockwise from x-axis
     */
    virtual std::tuple<common::Vector2, double> getPose() = 0;
};

}  // namespace hal

#endif
