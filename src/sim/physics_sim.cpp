#include "physics_sim.hpp"

#include <cmath>

#include "drivetrain.hpp"

namespace sim
{

PhysicsSim::PhysicsSim(HALProviderSimImpl *sim_hal)
    : sim_hal(sim_hal),
      odometry(robot::Drivetrain::wheel_base_width, robot::Drivetrain::wheel_circumference) { }

double PhysicsSim::estimate_angle_rotation(double time_delta) {
    HALProviderSimImpl::MotorAssembly *motors = sim_hal->motor_assembly();

    double left_speed = 0.5 * (motors->front_left()->get_speed() + motors->back_left()->get_speed());
    double right_speed = 0.5 * (motors->front_right()->get_speed() + motors->back_right()->get_speed());

    double delta_left_distance = left_speed * time_delta;
    double delta_right_distance = right_speed * time_delta;

    double speed = 0.5 * (left_speed + right_speed);

    double sum = delta_right_distance + delta_left_distance;
    double diff = delta_right_distance - delta_left_distance;

    if (std::abs(diff) < 1e-4) {
        return 0.0;
    }

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

    odometry.updateOdometry(sim_hal->gyroscope()->angle(), total_left_distance, total_right_distance);

    auto [position, heading] = odometry.getPose();

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

    sim_hal->lidar()->updateLocation(position, heading);
    sim_hal->cliff_infrared()->updateLocation(position, heading);
    sim_hal->wheel_infrared()->updateLocation(position, heading);
    sim_hal->ultrasonic()->updateLocation(position, heading);
}

}
