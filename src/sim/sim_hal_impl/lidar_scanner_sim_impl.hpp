#ifndef LIDAR_SCANNER_SIM_IMPL_HPP
#define LIDAR_SCANNER_SIM_IMPL_HPP

#include "lidar_scanner.hpp"
#include "distance_sensor_model.hpp"
#include "vector2.hpp"

namespace sim
{

class LidarScannerSimImpl final : public hal::LidarScanner {
public:
    LidarScannerSimImpl(DistanceSensorModel model, size_t points_per_scan);
    void updateLocation(common::Vector2 position, double heading);

    Scan getLatestScan() override;

private:
    DistanceSensorModel sensor_model;
    size_t points_per_scan;

    common::Vector2 position;
    double heading;
};

}

#endif
