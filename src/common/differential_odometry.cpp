#include "differential_odometry.hpp"

#include <cmath>

#include "common.hpp"

namespace common
{

DifferentialOdometry::DifferentialOdometry(double width, double wheel_circumference)
    : wheel_base_width(width), wheel_circumference(wheel_circumference), position(0.0, 0.0), heading(0.0) {}

void DifferentialOdometry::setPose(common::Vector2 position, double heading)
{
    this->position = position;
    this->heading = heading;
}

std::tuple<common::Vector2, double> DifferentialOdometry::getPose()
{
    return {position, heading};
}

void DifferentialOdometry::updateOdometry(double heading, double left_wheels_distance, double right_wheels_distance)
{
    double deltaLeft = left_wheels_distance - this->left_wheels_distance;
    double deltaRight = right_wheels_distance - this->right_wheels_distance;
    double deltaAngle = heading - this->heading;

    this->left_wheels_distance = left_wheels_distance;
    this->right_wheels_distance = right_wheels_distance;
    this->heading = heading;

    double distance = (deltaLeft + deltaRight) / 2.0;

    if (std::abs(deltaAngle) > PI)
    {
        deltaAngle = copysign(2.0 * PI - std::abs(deltaAngle), -deltaAngle);
    }

    if (std::abs(deltaAngle) < 1e-4)
    {
        double deltaX = distance * cos(heading);
        double deltaY = distance * sin(heading);

        position += common::Vector2(deltaX, deltaY);

        return;
    }

    double radius = distance / std::abs(deltaAngle);
    double chordAngle = heading - deltaAngle / 2.0; // average angle over turn
    double chordLength = 2.0 * radius * sin(std::abs(deltaAngle) / 2.0);

    double deltaX = chordLength * cos(chordAngle);
    double deltaY = chordLength * sin(chordAngle);

    position += common::Vector2(deltaX, deltaY);
}

}
