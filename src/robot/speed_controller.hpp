#ifndef SPEED_CONTROLLER_HPP
#define SPEED_CONTROLLER_HPP

#include "hal_provider.hpp"

namespace robot {

// Position of wheel relative to center of robot
class WheelPosition {
public:
    // Central angle around center of mass,
    // measured counter-clockwise from x-axis, radians
    double theta;

    // Distance from center of mass, meters
    double radius;

    WheelPosition(double theta, double radius);
};

class WheelPositions {
public:
    WheelPosition front_l;
    WheelPosition front_r;
    WheelPosition back_l;
    WheelPosition back_r;

    WheelPositions(WheelPosition front_l, WheelPosition front_r, WheelPosition back_l, WheelPosition back_r);
};

/**
 * Speed Controller the motor control loop uses to retrieve speed settings for
 * individual motors on the update cadence.
 */
class SpeedController {
public:
    /**
     * Construct with dependencies.
     *
     * @param wheel_positions position of each wheel relative to robot centre of mass
     * @param wheelDiameter diameter of the wheels in meters
     * @param max speed in meters per second
     */
    SpeedController(WheelPositions wheel_positions, double wheel_diameter, double max_speed, hal::HALProvider* hal);

    /**
     * Implementation of the updateSpeed method from the interface.
     *
     * @param gradient to follow
     */
    void updateSpeed(common::Vector2 gradient);

private:
    const WheelPositions wheel_positions; /**< wheel positions */
    const double wheel_diameter;          /**< diameters of the wheels on the robot */
    const double max_speed;

    hal::HALProvider::MotorAssembly* motors;
    hal::Positioning* positioning_system;

    /**
     * convert an angular velocity to a wheel rotation frequency
     *
     * @param angVel angular velocity to convert, radians per second
     * @param position polar location of the motor relative to centre of mass in meters
     * @return target frequency for given motor/wheel in revolutions per second
     */
    double angularToWheelFrequency(double angVel, WheelPosition position);
};

}  // namespace robot

#endif
