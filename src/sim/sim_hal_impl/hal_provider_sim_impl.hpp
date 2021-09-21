#ifndef HAL_PROVIDER_SIM_IMPL_HPP
#define HAL_PROVIDER_SIM_IMPL_HPP

#include <memory>
#include <vector>

#include "hal_provider.hpp"
#include "lidar_scanner.hpp"
#include "ultrasonic_sensor.hpp"
#include "infrared_sensor.hpp"

#include "lidar_scanner_sim_impl.hpp"
#include "ultrasonic_sensor_sim_impl.hpp"
#include "infrared_sensor_sim_impl.hpp"

namespace sim
{

class HALProviderSimImpl : public hal::HALProvider
{
public:
    HALProviderSimImpl();

    std::shared_ptr<hal::LidarScanner> lidar() override;
    std::shared_ptr<hal::UltrasonicSensor> ultrasonic(int id) override;
    std::shared_ptr<hal::InfraredSensor> infrared(int id) override;

private:
    double lidar_range;
    double ultrasonic_range;
    double infrared_range;

    BeamModel lidar_beam_model;
    BeamModel ultrasonic_beam_model;
    BeamModel infrared_beam_model;

    ObstacleMap obstacle_map;

    std::shared_ptr<LidarScannerSimImpl> lidar_impl;
    std::vector<std::shared_ptr<UltrasonicSensorSimImpl>> ultrasonic_sensors;
    std::vector<std::shared_ptr<InfraredSensorSimImpl>> infrared_sensors;
};

}

#endif
