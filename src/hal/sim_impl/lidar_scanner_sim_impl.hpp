#ifndef LIDAR_SCANNER_SIM_IMPL_HPP
#define LIDAR_SCANNER_SIM_IMPL_HPP

#include "hal/lidar_scanner.hpp"
#include "distance_sensor_model.hpp"
#include "common/vector2.hpp"
#include "sim/simulation.hpp"

namespace sim
{

class LidarScannerSimImpl final : public hal::LidarScanner {
public:
    LidarScannerSimImpl(sim::Simulation *simulation, DistanceSensorModel model, size_t points_per_scan);

    Scan getLatestScan() override;

private:
    sim::Simulation *simulation;
    DistanceSensorModel sensor_model;
    size_t points_per_scan;

    std::mt19937 gen;
};

}

#endif
