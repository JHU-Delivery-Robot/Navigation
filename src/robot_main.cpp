#include <functional>
#include <thread>

#include "common/periodic.hpp"
#include "comms/comms.hpp"
#include "events/event_queue.hpp"
#include "events/route_control.hpp"
#include "hal/robot_impl/hal_provider_impl.hpp"
#include "robot/config.hpp"
#include "robot/robot.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <robot config>" << std::endl;
        return 1;
    }

    std::filesystem::path config_file_path = std::filesystem::path(argv[1]);
    if (!std::filesystem::exists(config_file_path)) {
        std::cerr << "Can't find specified config file, please double-check path" << std::endl;
        return 1;
    }

    robot::Config config;
    std::cout << "Loading config... ";
    if (auto config_opt = robot::Config::load(config_file_path)) {
        std::cout << "Config loaded successfully" << std::endl;
        config = config_opt.value();
    } else {
        std::cerr << "Failed to load config" << std::endl;
        return 1;
    }

    hal::impl::HALProviderImpl::LidarConfig lidar_config = hal::impl::HALProviderImpl::LidarConfig(config.lidar_port, 12.0);

    events::EventQueue event_queue;
    events::ErrorReporting error_reporting(&event_queue);
    events::RouteControl route_control(&event_queue);

    hal::impl::HALProviderImpl hal_provider = hal::impl::HALProviderImpl(lidar_config, error_reporting);
    hal_provider.initialize();

    hal::impl::TG30Lidar* lidar = hal_provider.lidar();

    robot::Robot robot(&hal_provider, &event_queue);

    common::Periodic navigation("primary", 10, std::bind(&robot::Robot::update, &robot));

    comms::Comms::Credentials server_credentials(config.root_ca_cert, config.robot_cert, config.robot_key);
    comms::Comms comms(config.control_server_url, server_credentials, route_control, error_reporting, hal_provider.positioning());

    // Start
    lidar->beginScanning();
    comms.open();
    navigation.start();

    // Stop
    navigation.stop();
    lidar->stopScanning();
    comms.close();

    return 0;
}
