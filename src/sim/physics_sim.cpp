#include "physics_sim.hpp"

#include <cmath>

#include "drivetrain.hpp"

namespace sim {

PhysicsSim::PhysicsSim(HALProviderSimImpl *sim_hal)
    : sim_hal(sim_hal),
      odometry(robot::Drivetrain::wheel_base_width) {}

    double instantaneous_curve_radius = 0.5 * robot::Drivetrain::wheel_base_width * (sum / diff);

    if (std::abs(instantaneous_curve_radius) < 1e-4) {
        double delta_theta = 0.5*(delta_right_distance-delta_left_distance)/(PI*robot::Drivetrain::wheel_base_width);
        return delta_theta;
    }

    double distance = speed * time_delta;
    double delta_theta = distance / instantaneous_curve_radius;

    return delta_theta;
}

void PhysicsSim::update(double time_delta) {
    HALProviderSimImpl::MotorAssembly *motors = sim_hal->motor_assembly();

    motors->update(time_delta);

    double delta_theta = estimate_angle_rotation(time_delta);
    sim_hal->gyroscope()->updateHeading(delta_theta);

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
