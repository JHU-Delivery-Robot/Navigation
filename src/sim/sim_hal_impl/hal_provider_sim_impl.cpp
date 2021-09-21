#include "hal_provider_sim_impl.hpp"

namespace sim
{

constexpr double PI = 3.14159265358979323846;

HALProviderSimImpl::HALProviderSimImpl()
    : lidar_range(600.0), ultrasonic_range(200.0), infrared_range(100.0),
      lidar_beam_model(0.0, 0.0, lidar_range, 0.5, 0.5),
      ultrasonic_beam_model(0.0, 0.0, ultrasonic_range, 0.5, 0.5),
      infrared_beam_model(0.0, 0.0, infrared_range, 0.5, 0.5),
      obstacle_map({}) { }

std::shared_ptr<hal::LidarScanner> HALProviderSimImpl::lidar() {
    if (!lidar_impl) {
        auto sensorModel = DistanceSensorModel(obstacle_map, lidar_beam_model, lidar_range);
        lidar_impl = std::make_shared<LidarScannerSimImpl>(sensorModel);
    }

    return lidar_impl;
}

std::shared_ptr<hal::UltrasonicSensor> HALProviderSimImpl::ultrasonic(int id) {
    for (auto &&ultrasonic_sensor : ultrasonic_sensors)
    {
        if (ultrasonic_sensor->getID() == id) {
            return ultrasonic_sensor;
        }
    }
    
    auto sensorModel = DistanceSensorModel(obstacle_map, lidar_beam_model, lidar_range);
    auto ultrasonic_sensor = std::make_shared<UltrasonicSensorSimImpl>(id, sensorModel, PI/12, 5);
    ultrasonic_sensors.push_back(ultrasonic_sensor);

    return ultrasonic_sensor;
}

std::shared_ptr<hal::InfraredSensor> HALProviderSimImpl::infrared(int id) {
    for (auto &&infrared_sensor : infrared_sensors)
    {
        if (infrared_sensor->getID() == id) {
            return infrared_sensor;
        }
    }
    
    auto sensorModel = DistanceSensorModel(obstacle_map, lidar_beam_model, lidar_range);
    auto infrared_sensor = std::make_shared<InfraredSensorSimImpl>(id, sensorModel, PI/12, 5);
    infrared_sensors.push_back(infrared_sensor);

    return infrared_sensor;
}

}
