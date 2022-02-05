#include "lidar_scanner_sim_impl.hpp"

#include "common.hpp"
#include "robot.hpp"

namespace sim {

LidarScannerSimImpl::LidarScannerSimImpl(DistanceSensorModel model, size_t points_per_scan)
    : sensor_model(model), points_per_scan(points_per_scan), position(0.0, 0.0) {
        std::random_device random_device;
        gen.seed(random_device());
}

void LidarScannerSimImpl::updateLocation(common::Vector2 position, double heading) {
    this->position = position + common::Vector2::polar(heading, 0.5 * robot::Robot::length);
    this->heading = heading;
}

LidarScannerSimImpl::Scan LidarScannerSimImpl::getLatestScan() {
    Scan data = std::make_shared<std::vector<SamplePoint>>();

    double median_angle = 2 * PI / points_per_scan;

    std::normal_distribution<double> norm_dist(median_angle, 0.1 * median_angle);

    double curr_angle = 0.0;

    while (curr_angle < 2*PI) {
        common::Vector2 direction = common::Vector2::polar(curr_angle, 1.0);
        double distance = sensor_model.sample(position, direction);
        data->push_back(SamplePoint(distance, curr_angle));
        double angle = norm_dist(gen);
        curr_angle += angle;
    }

    return data;
}

}  // namespace sim