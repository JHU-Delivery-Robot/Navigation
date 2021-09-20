#include "speed_controller.hpp"

#include <cmath>

#include "potential_map.hpp"
#include "util.hpp"

namespace robot {

SpeedController::~SpeedController() {}

PotentialMapSpeedController::PotentialMapSpeedController(
    MotorPositions positions, float wheelDiameter,
    GradientPotentialMap* potenMap
) : motorPositions(positions),
    wheelDiameter{wheelDiameter},
    map{potenMap} {}

Speed PotentialMapSpeedController::angularToMotor(AngVel angVel, MotorPosition position) {
    return wheelDiameter * angVel * cos(position.theta / position.r);
}

void PotentialMapSpeedController::updateSpeed(common::Vector2 position, double heading) {
    common::Vector2 gradient = map->getGradient(position);
    Speed speed = gradient.dot(common::Vector2::polar(heading, 1.0));
    AngVel angVel = gradient.dot(common::Vector2::polar(heading + 0.5*PI, 1.0));

    motorSpeeds.front_l = speed + angularToMotor(angVel, motorPositions.front_l);
    motorSpeeds.back_l  = speed + angularToMotor(angVel, motorPositions.back_l);
    motorSpeeds.front_r = speed + angularToMotor(angVel, motorPositions.front_r);
    motorSpeeds.back_r  = speed + angularToMotor(angVel, motorPositions.back_r);
}

hal::MotorSpeeds PotentialMapSpeedController::getSpeedSettings() {
    return motorSpeeds;
}

}
