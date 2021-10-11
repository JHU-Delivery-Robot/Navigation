#ifndef DRIVETRAIN_HPP
#define DRIVETRAIN_HPP

#include "common.hpp"
#include "differential_odometry.hpp"
#include "hal_provider.hpp"
#include "vector2.hpp"
#include "potential_map.hpp"

namespace robot {

/**
 * Drivetrain computes individual motor speeds from high-level drive commands 
 */
class Drivetrain
{
public:
    Drivetrain(hal::HALProvider::MotorAssembly* motors, hal::Gyroscope* gyroscope);

    /**
     * Commands drivetrain so that the velocity vector of the robot matches
     * the specified drive command.
     * 
     * Note that it is the *middle of the front* of the robot that will have
     * the specified velocity vector, not the center, since we can't make the
     * center have a velocity vector that isn't parallel to the robot.
     */
    void setCommand(common::Vector2 drive_command);

    void setPose(common::Vector2 position, double heading);
    std::tuple<common::Vector2, double> getPose();

    void updateOdometry();

    // all in centimeters
    static constexpr double wheel_base_width = 40.0;
    static constexpr double length = 50.0;
    static constexpr double wheel_radius = 8;
    static constexpr double wheel_circumference = 2*PI*wheel_radius;
private:
    common::DifferentialOdometry odometry;

    hal::HALProvider::MotorAssembly* motors;
    hal::Gyroscope* gyroscope;
};

}

#endif
