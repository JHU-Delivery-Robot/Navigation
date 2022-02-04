#include "lidar_scanner_sim_impl.hpp"

#include "common.hpp"
#include "robot.hpp"

namespace sim {

LidarScannerSimImpl::LidarScannerSimImpl(DistanceSensorModel model, size_t points_per_scan)
    : sensor_model(model), points_per_scan(points_per_scan), position(0.0, 0.0) {}

void LidarScannerSimImpl::updateLocation(common::Vector2 position, double heading) {
    this->position = position + common::Vector2::polar(heading, 0.5 * robot::Robot::length);
    this->heading = heading;
}

LidarScannerSimImpl::Scan LidarScannerSimImpl::getLatestScan() {
    Scan data = std::make_shared<std::vector<SamplePoint>>();

    for (std::size_t i = 0; i < points_per_scan; i++) {
        double angle = heading + 2 * PI * i / double(points_per_scan);
        common::Vector2 direction = common::Vector2::polar(angle, 1.0);
        double distance = sensor_model.sample(position, direction);
        data->push_back(SamplePoint(distance, angle));
    }

    return data;
}

}  // namespace sim
