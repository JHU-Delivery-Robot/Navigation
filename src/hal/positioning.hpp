#ifndef POSITIONING_HPP
#define POSITIONING_HPP

#include <tuple>

#include "vector2.hpp"

namespace hal {

class Positioning {
public:
    virtual ~Positioning() {}

    /**
     * Get the current pose of the robot,
     * as position in meters and angle in radians
     */
    virtual std::tuple<common::Vector2, double> getPose() = 0;

    /**
     * Set the pose of the robot
     * 
     * @param position of the robot in meters
     * @param heading of the robot in radians, measured counter-clockwise from x-axis
     */
    virtual void setPose(common::Vector2 position, double heading) = 0;
};

}  // namespace hal

#endif
