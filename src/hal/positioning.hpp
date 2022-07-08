#ifndef POSITIONING_HPP
#define POSITIONING_HPP

#include <tuple>

#include "common/coordinate_system.hpp"
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
    virtual std::tuple<common::Vector2, double> getPose() const = 0;

    /**
     * Retrieve coordinate system used by positioning system.
     */
    virtual common::CoordinateSystem coordinateSystem() const = 0;
};

}  // namespace hal

#endif
