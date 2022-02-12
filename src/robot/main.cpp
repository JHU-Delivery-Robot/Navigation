#include <functional>
#include <thread>

#include "event_queue.hpp"
#include "hal_provider_impl.hpp"
#include "loop.hpp"
#include "robot.hpp"

int main() {
    //Initialize configs
    hal::impl::HALProviderImpl::LidarConfig lidar_config =
        hal::impl::HALProviderImpl::LidarConfig("COM3", 12.0);

    //Initialize hal provider implementation
    hal::impl::HALProviderImpl hal_provider = hal::impl::HALProviderImpl(lidar_config);
    hal_provider.initialize();

    //Get Lidar
    hal::impl::TG30Lidar* lidar = hal_provider.lidar();

    events::EventQueue event_queue;

    //Get Robot
    robot::Robot rob = robot::Robot(&hal_provider, &event_queue);

    //Initialize Robot Loop
    robot::Loop robot_loop = robot::Loop(10, std::bind(&robot::Robot::update, &rob));

    //Start all threads
    std::thread robot_thread(&robot::Loop::looping, &robot_loop);  //Robot Thread
    lidar->beginScanning();                                        //Lidar Thread

    // TODO: Insert Stopping Function

    //Stop all threads
    robot_loop.cancel();
    robot_thread.join();
    lidar->stopScanning();

    return 0;
}
