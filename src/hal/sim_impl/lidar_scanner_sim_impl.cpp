#include "lidar_scanner_sim_impl.hpp"

#include "common/common.hpp"
#include "robot/robot.hpp"

namespace sim {

LidarScannerSimImpl::LidarScannerSimImpl(sim::Simulation *simulation, DistanceSensorModel model, size_t points_per_scan)
    : simulation(simulation), sensor_model(model), points_per_scan(points_per_scan) {
        std::random_device random_device;
        gen.seed(random_device());
}

LidarScannerSimImpl::Scan LidarScannerSimImpl::getLatestScan() {
    Scan data = std::make_shared<std::vector<SamplePoint>>();

    double average_angle_delta = 2 * PI / points_per_scan;
    std::normal_distribution<double> angle_distribution(average_angle_delta, 0.1 * average_angle_delta);

    auto [position, angle] = simulation->getPose();

    double sample_angle = 0.0;
    while (sample_angle < 2 * PI) {
        common::Vector2 direction = common::Vector2::polar(sample_angle, 1.0);
        double distance = sensor_model.sample(position, direction);
        data->push_back(SamplePoint(distance, sample_angle));
        double angle_delta = angle_distribution(gen);
        sample_angle += angle_delta;
    }

    return data;
}

}  // namespace sim