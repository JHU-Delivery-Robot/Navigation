#ifndef HAL_PROVIDER_HPP
#define HAL_PROVIDER_HPP

#include <memory>

#include "lidar_scanner.hpp"
#include "ultrasonic_sensor.hpp"
#include "infrared_sensor.hpp"

namespace hal
{

class HALProvider
{
public:
    virtual ~HALProvider();

    virtual std::shared_ptr<LidarScanner> lidar() = 0;
    virtual std::shared_ptr<UltrasonicSensor> ultrasonic(int id) = 0;
    virtual std::shared_ptr<InfraredSensor> infrared(int id) = 0;
};

}

#endif
