#ifndef MOTORS_HPP
#define MOTORS_HPP

#include "hal.hpp"
#include "types.hpp"

/**
 * Speed Controller the motor control loop uses to retrieve speed settings for
 * individual motors ont he update cadence.
 */
class SpeedController {
    /**
     * Update internal motor speed states using a tearget speed and angular
     * velocity.
     */
    virtual void updateSpeed(Speed speed, AngVel angVel) = 0;
    /**
     * Get speed settings for all motors on the robot
     */
    virtual hal::MotorSpeeds getSpeedSettings() = 0;
};


class PotentialMapSpeedController : public SpeedController {
private:
    const MotorPositions motorPositions; /**< motor positions in rdaial coords for all motors */
    const float wheelDiameter;           /**< diameters of the wheels on the robot */
    hal::MotorSpeeds motorSpeeds;        /**< individual motor speeds */
    /**
     * convert an angular velocity to a motor specific velocity
     *
     * @param ang_vel angular velocity to convert
     * @param position porlar location of the motor relative to centre of mass
     * @return target velocity for given motor
     */
    Speed angularToMotor(AngVel ang_vel, MotorPosition position);
public:
    /**
     * Construct with dependencies.
     *
     * @param positions MotorPositions oject containing the position of each
     *                  motor in the robot
     * @param wheelDiameter diameter of the wheels in centimeters
     */
    PotentialMapSpeedController(MotorPositions positions, float wheelDiameter);
    /**
     * Implementation of the updateSpeed method from the interface.
     *
     * @param speed linear speed target in ???
     * @para ang_vel angular speed target in ??
     */
    void updateSpeed(Speed speed, AngVel ang_vel);
    /**
     * Implementation of the getSpeedSettings method from the interface.
     *
     * @return MotorSpeeds object containing speed setpoint fro each motor
     */
    hal::MotorSpeeds getSpeedSettings();
};

#endif
