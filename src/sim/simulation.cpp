#include "simulation.hpp"

#include <cmath>
#include <vector>

namespace sim {

Simulation::Simulation(Config config)
    : obstacle_map(config.obstacles), simulation_time(0.0), position(config.start_position), linear_velocity(common::Vector2(0.0, 0.0)), angle(config.start_angle), angular_velocity(0.0) {}

void Simulation::update() {
    double delta_angle = config.time_step * angular_velocity;
    double average_angle = angle + linear_velocity.angle() + 0.5 * delta_angle;
    common::Vector2 delta_position = common::Vector2::polar(average_angle, config.time_step * linear_velocity.magnitude());

    position += delta_position;
    angle += delta_angle;

    simulation_time += config.time_step;
}

std::tuple<common::Vector2, double> Simulation::getPose() const {
    return {position, angle};
}

void Simulation::setVelocity(common::Vector2 linear_velocity, double angular_velocity) {
    this->linear_velocity = linear_velocity;
    this->angular_velocity = angular_velocity;
}

double Simulation::elapsedTime() const {
    return simulation_time;
}

const ObstacleMap* Simulation::obstacles() const {
    return &obstacle_map;
}

}  // namespace sim
