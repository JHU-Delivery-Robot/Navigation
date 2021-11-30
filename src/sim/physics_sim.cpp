#include "physics_sim.hpp"

#include <cmath>

#include "drivetrain.hpp"

namespace sim {

PhysicsSim::PhysicsSim(HALProviderSimImpl *sim_hal)
    : sim_hal(sim_hal),
      odometry(robot::Drivetrain::wheel_base_width) {}

void PhysicsSim::update(double time_delta) {
    HALProviderSimImpl::MotorAssembly *motors = sim_hal->motor_assembly();

    motors->update(time_delta);

    double total_left_distance = 0.5 * (motors->front_left()->odometry_distance() + motors->back_left()->odometry_distance());
    double total_right_distance = 0.5 * (motors->front_right()->odometry_distance() + motors->back_right()->odometry_distance());

    double delta_theta = odometry.estimateRotation(total_left_distance, total_right_distance);
    sim_hal->gyroscope()->updateHeading(delta_theta);

    odometry.updateOdometry(sim_hal->gyroscope()->angle(), total_left_distance, total_right_distance);

    auto [position, heading] = odometry.getPose();

    sim_hal->gps()->updateLocation(position, heading);
    sim_hal->lidar()->updateLocation(position, heading);
    sim_hal->cliff_infrared()->updateLocation(position, heading);
    sim_hal->wheel_infrared()->updateLocation(position, heading);
    sim_hal->ultrasonic()->updateLocation(position, heading);
}

std::tuple<common::Vector2, double> PhysicsSim::getPose() {
    return odometry.getPose();
}

void PhysicsSim::setPose(common::Vector2 position, double heading) {
    odometry.setPose(position, heading);
    sim_hal->gyroscope()->setHeading(heading);

    sim_hal->gps()->updateLocation(position, heading);
    sim_hal->lidar()->updateLocation(position, heading);
    sim_hal->cliff_infrared()->updateLocation(position, heading);
    sim_hal->wheel_infrared()->updateLocation(position, heading);
    sim_hal->ultrasonic()->updateLocation(position, heading);
}

}  // namespace sim
