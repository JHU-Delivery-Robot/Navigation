#include "speed_controller.hpp"

#include <cmath>
#include "poten_map.hpp"

PotentialMapSpeedController::PotentialMapSpeedController(MotorPositions positions, float wheelDiameter) :
    motorPositions(positions),
    wheelDiameter{wheelDiameter} {}

Speed PotentialMapSpeedController::angularToMotor(AngVel ang_vel, MotorPosition position) {
    // theta is angle from horizontal angle of inclination to the motors
    // Linear distance from centre of mass to the motors
    return wheelDiameter * ang_vel * cos(position.theta / position.r);
}

void PotentialMapSpeedController::updateSpeed(Speed speed, AngVel ang_vel) {
    motorSpeeds.front_l = angularToMotor(ang_vel, motorPositions.front_l);
    motorSpeeds.back_l  = angularToMotor(ang_vel, motorPositions.back_l);
    motorSpeeds.front_r = angularToMotor(ang_vel, motorPositions.front_r);
    motorSpeeds.back_r  = angularToMotor(ang_vel, motorPositions.back_r);
}

hal::MotorSpeeds PotentialMapSpeedController::getSpeedSettings() {
    return motorSpeeds;
}
