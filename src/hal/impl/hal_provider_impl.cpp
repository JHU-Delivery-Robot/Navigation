#include "hal_provider_impl.hpp"

#include <string>

#include "TG30_lidar.hpp"

namespace hal {

namespace impl {

HALProviderImpl::LidarConfig::LidarConfig(std::string port, double scan_frequency)
    : port(port), scan_frequency(scan_frequency) {}

InfraredSensor* HALProviderImpl::CliffInfraredAssembly::front() {
    return nullptr;
}

InfraredSensor* HALProviderImpl::CliffInfraredAssembly::left() {
    return nullptr;
}

InfraredSensor* HALProviderImpl::CliffInfraredAssembly::right() {
    return nullptr;
}

InfraredSensor* HALProviderImpl::CliffInfraredAssembly::back() {
    return nullptr;
}

InfraredSensor* HALProviderImpl::WheelInfraredAssembly::front_left() {
    return nullptr;
}

InfraredSensor* HALProviderImpl::WheelInfraredAssembly::front_right() {
    return nullptr;
}

InfraredSensor* HALProviderImpl::WheelInfraredAssembly::back_left() {
    return nullptr;
}

InfraredSensor* HALProviderImpl::WheelInfraredAssembly::back_right() {
    return nullptr;
}

UltrasonicSensor* HALProviderImpl::UltrasonicAssembly::front() {
    return nullptr;
}

UltrasonicSensor* HALProviderImpl::UltrasonicAssembly::left() {
    return nullptr;
}

UltrasonicSensor* HALProviderImpl::UltrasonicAssembly::right() {
    return nullptr;
}

UltrasonicSensor* HALProviderImpl::UltrasonicAssembly::back() {
    return nullptr;
}

HALProviderImpl::MotorAssembly::MotorAssembly() {}

Motor* HALProviderImpl::MotorAssembly::front_left() {
    return nullptr;
}

Motor* HALProviderImpl::MotorAssembly::front_right() {
    return nullptr;
}

Motor* HALProviderImpl::MotorAssembly::back_left() {
    return nullptr;
}

Motor* HALProviderImpl::MotorAssembly::back_right() {
    return nullptr;
}

void HALProviderImpl::MotorAssembly::reset_odometry() {}

TG30Lidar* HALProviderImpl::lidar() {
    return &lidar_impl;
}

HALProviderImpl::CliffInfraredAssembly* HALProviderImpl::cliff_infrared() {
    return &cliff_sensors;
}

HALProviderImpl::WheelInfraredAssembly* HALProviderImpl::wheel_infrared() {
    return &wheel_sensors;
}

HALProviderImpl::UltrasonicAssembly* HALProviderImpl::ultrasonic() {
    return &ultrasonic_sensors;
}

HALProviderImpl::MotorAssembly* HALProviderImpl::motor_assembly() {
    return &motors;
}

Gyroscope* HALProviderImpl::gyroscope() {
    return nullptr;
}

GPS* HALProviderImpl::gps() {
    return nullptr;
}

HALProviderImpl::HALProviderImpl(LidarConfig config) : config(config) {}

void HALProviderImpl::initialize() {
    // TODO: Need to setup config that holds port details, etc.
    lidar_impl.initialize(config.port);
    lidar_impl.setScanFrequency(config.scan_frequency);
}

}  // namespace impl

}  // namespace hal
