#include "hal_provider_sim_impl.hpp"

#include "robot/robot.hpp"

namespace sim {

DistanceSensorModel HALProviderSimImpl::CliffInfraredAssembly::construct_sensor_model(sim::Simulation* simulation) {
    BeamModel beam_model = BeamModel(0.0, max_range, 0.0, 0.0);

    return DistanceSensorModel(simulation->obstacles(), beam_model, max_range);
}

HALProviderSimImpl::CliffInfraredAssembly::CliffInfraredAssembly(sim::Simulation* simulation)
    : _front(0, simulation, construct_sensor_model(simulation), fov, samples),
      _left(1, simulation, construct_sensor_model(simulation), fov, samples),
      _right(2, simulation, construct_sensor_model(simulation), fov, samples),
      _back(3, simulation, construct_sensor_model(simulation), fov, samples) {}

InfraredSensorSimImpl* HALProviderSimImpl::CliffInfraredAssembly::front() {
    return &_front;
}

InfraredSensorSimImpl* HALProviderSimImpl::CliffInfraredAssembly::left() {
    return &_left;
}

InfraredSensorSimImpl* HALProviderSimImpl::CliffInfraredAssembly::right() {
    return &_right;
}

InfraredSensorSimImpl* HALProviderSimImpl::CliffInfraredAssembly::back() {
    return &_back;
}

DistanceSensorModel HALProviderSimImpl::WheelInfraredAssembly::construct_sensor_model(sim::Simulation* simulation) {
    BeamModel beam_model = BeamModel(0.0, max_range, 0.0, 0.0);

    return DistanceSensorModel(simulation->obstacles(), beam_model, max_range);
}

HALProviderSimImpl::WheelInfraredAssembly::WheelInfraredAssembly(sim::Simulation* simulation)
    : _front_left(0, simulation, construct_sensor_model(simulation), fov, samples),
      _front_right(1, simulation, construct_sensor_model(simulation), fov, samples),
      _back_left(2, simulation, construct_sensor_model(simulation), fov, samples),
      _back_right(3, simulation, construct_sensor_model(simulation), fov, samples) {}

InfraredSensorSimImpl* HALProviderSimImpl::WheelInfraredAssembly::front_left() {
    return &_front_left;
}

InfraredSensorSimImpl* HALProviderSimImpl::WheelInfraredAssembly::front_right() {
    return &_front_right;
}

InfraredSensorSimImpl* HALProviderSimImpl::WheelInfraredAssembly::back_left() {
    return &_back_left;
}

InfraredSensorSimImpl* HALProviderSimImpl::WheelInfraredAssembly::back_right() {
    return &_back_right;
}

DistanceSensorModel HALProviderSimImpl::UltrasonicAssembly::construct_sensor_model(sim::Simulation* simulation) {
    BeamModel beam_model = BeamModel(0.0, max_range, 0.0, 0.0);

    return DistanceSensorModel(simulation->obstacles(), beam_model, max_range);
}

HALProviderSimImpl::UltrasonicAssembly::UltrasonicAssembly(sim::Simulation* simulation)
    : _front(0, simulation, construct_sensor_model(simulation), fov, samples),
      _left(1, simulation, construct_sensor_model(simulation), fov, samples),
      _right(2, simulation, construct_sensor_model(simulation), fov, samples),
      _back(3, simulation, construct_sensor_model(simulation), fov, samples) {}

UltrasonicSensorSimImpl* HALProviderSimImpl::UltrasonicAssembly::front() {
    return &_front;
}

UltrasonicSensorSimImpl* HALProviderSimImpl::UltrasonicAssembly::left() {
    return &_left;
}

UltrasonicSensorSimImpl* HALProviderSimImpl::UltrasonicAssembly::right() {
    return &_right;
}

UltrasonicSensorSimImpl* HALProviderSimImpl::UltrasonicAssembly::back() {
    return &_back;
}

HALProviderSimImpl::MotorAssembly::MotorAssembly(sim::Simulation* simulation) : simulation(simulation) {}

MotorSimImpl* HALProviderSimImpl::MotorAssembly::front_left() {
    return &_front_left;
}

MotorSimImpl* HALProviderSimImpl::MotorAssembly::front_right() {
    return &_front_right;
}

MotorSimImpl* HALProviderSimImpl::MotorAssembly::back_left() {
    return &_back_left;
}

MotorSimImpl* HALProviderSimImpl::MotorAssembly::back_right() {
    return &_back_right;
}

void HALProviderSimImpl::MotorAssembly::update() {
    double time_delta = simulation->elapsedTime() - last_update_time;
    last_update_time = simulation->elapsedTime();

    _front_left.update(time_delta);
    _front_right.update(time_delta);
    _back_left.update(time_delta);
    _back_right.update(time_delta);

    double left_speed = 0.5 * (_front_left.get_speed() + _back_left.get_speed());
    double right_speed = 0.5 * (_front_right.get_speed() + _back_right.get_speed());

    common::Vector2 linear_velocity = common::Vector2::polar(0.0, 0.5 * (left_speed + right_speed));
    double angular_velocity = (right_speed - left_speed) / robot::Robot::wheel_base_width;

    simulation->setVelocity(linear_velocity, angular_velocity);
}

void HALProviderSimImpl::MotorAssembly::reset_odometry() {
    _front_left.reset_odometry();
    _front_right.reset_odometry();
    _back_left.reset_odometry();
    _back_right.reset_odometry();
}

LidarScannerSimImpl* HALProviderSimImpl::lidar() {
    return &lidar_impl;
}

HALProviderSimImpl::CliffInfraredAssembly* HALProviderSimImpl::cliff_infrared() {
    return &cliff_sensors;
}

HALProviderSimImpl::WheelInfraredAssembly* HALProviderSimImpl::wheel_infrared() {
    return &wheel_sensors;
}

HALProviderSimImpl::UltrasonicAssembly* HALProviderSimImpl::ultrasonic() {
    return &ultrasonic_sensors;
}

HALProviderSimImpl::MotorAssembly* HALProviderSimImpl::motor_assembly() {
    return &motors;
}

PositioningSimImpl* HALProviderSimImpl::positioning() {
    return &positioning_impl;
}

HALProviderSimImpl::HALProviderSimImpl(sim::Simulation* simulation, common::CoordinateSystem coordinate_system)
    : lidar_beam_model(0.0, lidar_max_range, 0.0, 0.0),
      lidar_impl(simulation, DistanceSensorModel(simulation->obstacles(), lidar_beam_model, lidar_max_range), 1000),
      cliff_sensors(simulation),
      wheel_sensors(simulation),
      ultrasonic_sensors(simulation),
      motors(simulation),
      positioning_impl(simulation, coordinate_system) {}

void HALProviderSimImpl::update() {
    motors.update();
}

}  // namespace sim
