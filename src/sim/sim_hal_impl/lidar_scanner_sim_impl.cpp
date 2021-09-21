#include "lidar_scanner_sim_impl.hpp"

namespace sim
{

LidarScannerSimImpl::LidarScannerSimImpl(DistanceSensorModel model)
    : sensor_model(model), position(0.0, 0.0), direction(common::Vector2::polar(0.0, 1.0)) { }

void LidarScannerSimImpl::updateLocation(common::Vector2 position, double heading) {
    this->position = position;
    this->direction = common::Vector2::polar(heading, 1.0);
}

std::array<double, LidarScannerSimImpl::SamplesPerRevolution> LidarScannerSimImpl::read() {
    std::array<double, LidarScannerSimImpl::SamplesPerRevolution> data;

    for (size_t i = 0; i < SamplesPerRevolution; i++) {
        data[i] = sensor_model.sample(position, direction);
    }

    return data;
}

}
