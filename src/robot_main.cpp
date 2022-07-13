#include <functional>
#include <thread>

#include "common/periodic.hpp"
#include "comms/comms.hpp"
#include "events/event_queue.hpp"
#include "events/route_control.hpp"
#include "hal/robot_impl/hal_provider_impl.hpp"
#include "robot/robot.hpp"

int main() {
    hal::impl::HALProviderImpl::LidarConfig lidar_config = hal::impl::HALProviderImpl::LidarConfig("COM3", 12.0);

    events::EventQueue event_queue;
    events::ErrorReporting error_reporting(&event_queue);
    events::RouteControl route_control(&event_queue);

    hal::impl::HALProviderImpl hal_provider = hal::impl::HALProviderImpl(lidar_config, error_reporting);
    hal_provider.initialize();

    hal::impl::TG30Lidar* lidar = hal_provider.lidar();

    robot::Robot robot(&hal_provider, &event_queue);

    common::Periodic navigation("primary", 10, std::bind(&robot::Robot::update, &robot));

    comms::Comms comms("127.0.0.1:9000", route_control, error_reporting, hal_provider.positioning());

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
