#ifndef SIM_HPP
#define SIM_HPP

#include <filesystem>
#include <tuple>

#include "hal/hal_provider.hpp"
#include "common/vector2.hpp"
#include "obstacle_map.hpp"
#include "config.hpp"

namespace sim {

/**
 * Core of the robot simulation. Tracks robot's position and velocity and manages the simplistic physics update,
 * and provides this info to the simulation HAL.
 */
class Simulation {
public:
    Simulation(Config config);

    // Get current robot position and angle
    std::tuple<common::Vector2, double> getPose() const;

    // Set current velocity. Linear velocity is relative to robot's orientation,
    // and positive angular velocity is anti-clockwise.
    void setVelocity(common::Vector2 linear_velocity, double angular_velocity);

    // Get time in second elapsed in-simulation since simulation began
    double elapsedTime() const;

    const ObstacleMap* obstacles() const;

    // Step simulation forward
    void update();

private:
    Config config;
    ObstacleMap obstacle_map;

    double simulation_time;
    common::Vector2 position;
    common::Vector2 linear_velocity;

    double angle;
    double angular_velocity;
};

}  // namespace sim

#endif
