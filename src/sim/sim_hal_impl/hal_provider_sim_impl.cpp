#include "hal_provider_sim_impl.hpp"

namespace sim {

DistanceSensorModel HALProviderSimImpl::CliffInfraredAssembly::construct_sensor_model(ObstacleMap* obstacle_map) {
    BeamModel beam_model = BeamModel(0.0, 0.0, max_range, 0.0, 0.0);

    return DistanceSensorModel(obstacle_map, beam_model, max_range);
}

HALProviderSimImpl::CliffInfraredAssembly::CliffInfraredAssembly(ObstacleMap* obstacle_map)
    : _front(0, construct_sensor_model(obstacle_map), fov, samples),
      _left(1, construct_sensor_model(obstacle_map), fov, samples),
      _right(2, construct_sensor_model(obstacle_map), fov, samples),
      _back(3, construct_sensor_model(obstacle_map), fov, samples) {}

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

void HALProviderSimImpl::CliffInfraredAssembly::updateLocation(common::Vector2 position, double heading) {
    _front.updateLocation(position, heading);
    _left.updateLocation(position, heading);
    _right.updateLocation(position, heading);
    _back.updateLocation(position, heading);
}

DistanceSensorModel HALProviderSimImpl::WheelInfraredAssembly::construct_sensor_model(ObstacleMap* obstacle_map) {
    BeamModel beam_model = BeamModel(0.0, 0.0, max_range, 0.0, 0.0);

    return DistanceSensorModel(obstacle_map, beam_model, max_range);
}

HALProviderSimImpl::WheelInfraredAssembly::WheelInfraredAssembly(ObstacleMap* obstacle_map)
    : _front_left(0, construct_sensor_model(obstacle_map), fov, samples),
      _front_right(1, construct_sensor_model(obstacle_map), fov, samples),
      _back_left(2, construct_sensor_model(obstacle_map), fov, samples),
      _back_right(3, construct_sensor_model(obstacle_map), fov, samples) {}

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

void HALProviderSimImpl::WheelInfraredAssembly::updateLocation(common::Vector2 position, double heading) {
    _front_left.updateLocation(position, heading);
    _front_right.updateLocation(position, heading);
    _back_left.updateLocation(position, heading);
    _back_right.updateLocation(position, heading);
}

DistanceSensorModel HALProviderSimImpl::UltrasonicAssembly::construct_sensor_model(ObstacleMap* obstacle_map) {
    BeamModel beam_model = BeamModel(0.0, 0.0, max_range, 0.0, 0.0);

    return DistanceSensorModel(obstacle_map, beam_model, max_range);
}

HALProviderSimImpl::UltrasonicAssembly::UltrasonicAssembly(ObstacleMap* obstacle_map)
    : _front(0, construct_sensor_model(obstacle_map), fov, samples),
      _left(1, construct_sensor_model(obstacle_map), fov, samples),
      _right(2, construct_sensor_model(obstacle_map), fov, samples),
      _back(3, construct_sensor_model(obstacle_map), fov, samples) {}

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

void HALProviderSimImpl::UltrasonicAssembly::updateLocation(common::Vector2 position, double heading) {
    _front.updateLocation(position, heading);
    _left.updateLocation(position, heading);
    _right.updateLocation(position, heading);
    _back.updateLocation(position, heading);
}

HALProviderSimImpl::MotorAssembly::MotorAssembly() {}

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

void HALProviderSimImpl::MotorAssembly::update(double time_delta) {
    _front_left.update(time_delta);
    _front_right.update(time_delta);
    _back_left.update(time_delta);
    _back_right.update(time_delta);
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

GyroscopeSimImpl* HALProviderSimImpl::gyroscope() {
    return &gyroscope_impl;
}

GPSSimImpl* HALProviderSimImpl::gps() {
    return &gps_impl;
}

HALProviderSimImpl::HALProviderSimImpl(std::vector<Polygon> obstacles)
    : lidar_beam_model(0.0, 0.0, lidar_max_range, 0.0, 0.0),
      obstacle_map(obstacles),
      lidar_impl(DistanceSensorModel(&obstacle_map, lidar_beam_model, lidar_max_range), 1000),
      cliff_sensors(&obstacle_map),
      wheel_sensors(&obstacle_map),
      ultrasonic_sensors(&obstacle_map),
      motors() {}

}  // namespace sim
