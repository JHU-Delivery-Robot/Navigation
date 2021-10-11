#include "lidar_scanner_sim_impl.hpp"

//#include <iostream>

#include "common.hpp"

namespace sim {

LidarScannerSimImpl::LidarScannerSimImpl(DistanceSensorModel model)
    : sensor_model(model), position(0.0, 0.0) {}

void LidarScannerSimImpl::updateLocation(common::Vector2 position, double heading) {
    this->position = position + common::Vector2::polar(heading, 25);
    this->heading = heading;
}

std::array<double, LidarScannerSimImpl::SamplesPerRevolution> LidarScannerSimImpl::read() {
    std::array<double, LidarScannerSimImpl::SamplesPerRevolution> data;

    for (size_t i = 0; i < SamplesPerRevolution; i++) {
        common::Vector2 direction = common::Vector2::polar(heading + 2 * PI * i / double(SamplesPerRevolution), 1.0);
        data[i] = sensor_model.sample(position, direction);
    }

    return data;
}

}  // namespace sim
