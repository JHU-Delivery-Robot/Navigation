#include "physics_sim.hpp"

#include <cmath>

#include "robot.hpp"

namespace sim {

PhysicsSim::PhysicsSim(hal::HALProvider *hal)
    : hal(hal),
      odometry(robot::Robot::wheel_base_width) {}

void PhysicsSim::update() {
    hal::HALProvider::MotorAssembly *motors = hal->motor_assembly();

    double total_left_distance = 0.5 * (motors->front_left()->odometry_distance() + motors->back_left()->odometry_distance());
    double total_right_distance = 0.5 * (motors->front_right()->odometry_distance() + motors->back_right()->odometry_distance());

    double delta_theta = odometry.estimateRotation(total_left_distance, total_right_distance);
    odometry.updateOdometry(hal->gyroscope()->angle() + delta_theta, total_left_distance, total_right_distance);
}

std::tuple<common::Vector2, double> PhysicsSim::getPose() {
    return odometry.getPose();
}

void PhysicsSim::setPose(common::Vector2 position, double heading) {
    odometry.setPose(position, heading);
}

}  // namespace sim
