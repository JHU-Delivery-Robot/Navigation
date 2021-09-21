#ifndef LIDAR_SCANNER_SIM_IMPL_HPP
#define LIDAR_SCANNER_SIM_IMPL_HPP

#include "lidar_scanner.hpp"
#include "distance_sensor_model.hpp"
#include "vector2.hpp"

namespace sim
{

class LidarScannerSimImpl : public hal::LidarScanner {
public:
    LidarScannerSimImpl(DistanceSensorModel model);
    void updateLocation(common::Vector2 position, double heading);

    std::array<double, SamplesPerRevolution> read() override;

private:
    DistanceSensorModel sensor_model;

    common::Vector2 position;
    common::Vector2 direction;
};

}

#endif
