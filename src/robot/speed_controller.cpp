#include "speed_controller.hpp"

#include <cmath>
#include "potential_map.hpp"

SpeedController::~SpeedController() {}

PotentialMapSpeedController::PotentialMapSpeedController(MotorPositions positions, float wheelDiameter,
    GradientPotentialMap* potenMap) :
    motorPositions(positions),
    wheelDiameter{wheelDiameter},
    map{potenMap} {}

Speed PotentialMapSpeedController::angularToMotor(AngVel angVel, MotorPosition position) {
    return wheelDiameter * angVel * cos(position.theta / position.r);
}

void PotentialMapSpeedController::updateSpeed(Vec2d position, double heading) {
    Speed speed = map->getRadialGradient(position, heading);
    AngVel angVel = map->getTangentGradient(position, heading);
    motorSpeeds.front_l = speed + angularToMotor(angVel, motorPositions.front_l);
    motorSpeeds.back_l  = speed + angularToMotor(angVel, motorPositions.back_l);
    motorSpeeds.front_r = speed + angularToMotor(angVel, motorPositions.front_r);
    motorSpeeds.back_r  = speed + angularToMotor(angVel, motorPositions.back_r);
}

hal::MotorSpeeds PotentialMapSpeedController::getSpeedSettings() {
    return motorSpeeds;
}
