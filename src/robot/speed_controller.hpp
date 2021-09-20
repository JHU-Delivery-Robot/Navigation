#ifndef MOTORS_HPP
#define MOTORS_HPP

#include "hal.hpp"
#include "vector2.hpp"
#include "types.hpp"
#include "potential_map.hpp"
#include "types.hpp"

/**
 * Speed Controller the motor control loop uses to retrieve speed settings for
 * individual motors ont he update cadence.
 */
class SpeedController {
public:
    virtual ~SpeedController();

    /**
     * Update internal motor speed states using a target speed and angular
     * velocity.
     */
    virtual void updateSpeed(Speed speed, AngVel angVel) = 0;  //EDIT THIS TO GET CURRENT SPEED
    /**
     * Get speed settings for all motors on the robot
     */
    virtual hal::MotorSpeeds getSpeedSettings() = 0;
};

class PotentialMapSpeedController : public SpeedController {
private:
    const MotorPositions motorPositions; /**< motor positions in radial coords for all motors */
    const float wheelDiameter;           /**< diameters of the wheels on the robot */
    hal::MotorSpeeds motorSpeeds;        /**< individual motor speeds */
    GradientPotentialMap *map;           /**< potential map */
    /**
     * convert an angular velocity to a motor specific velocity
     *
     * @param angVel angular velocity to convert
     * @param position polar location of the motor relative to centre of mass
     * @return target velocity for given motor
     */
    Speed angularToMotor(AngVel angVel, MotorPosition position);

public:
    /**
     * Construct with dependencies.
     *
     * @param positions MotorPositions object containing the position of each
     *                  motor in the robot
     * @param wheelDiameter diameter of the wheels in centimeters
     */
    PotentialMapSpeedController(MotorPositions positions, float wheelDiameter, GradientPotentialMap *potenMap);
    /**
     * Implementation of the updateSpeed method from the interface.
     *
     * @param speed linear speed target in ???
     * @para angVel angular speed target in ??
     */
    void updateSpeed(common::Vector2 position, double heading);
    /**
     * Implementation of the getSpeedSettings method from the interface.
     *
     * @return MotorSpeeds object containing speed setpoint for each motor
     */
    hal::MotorSpeeds getSpeedSettings();
};

#endif
