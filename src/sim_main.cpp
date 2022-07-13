#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <chrono>
#include <thread>

#include "comms/comms.hpp"
#include "events/event_queue.hpp"
#include "hal/sim_impl/hal_provider_sim_impl.hpp"
#include "robot/robot.hpp"
#include "sim/config.hpp"
#include "sim/obstacle_map.hpp"
#include "sim/polygon.hpp"
#include "sim/recording.hpp"
#include "sim/simulation.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Please specify config file path as only argument" << std::endl;
        return 1;
    }

    std::filesystem::path config_file_path = std::filesystem::path(argv[1]);
    if (!std::filesystem::exists(config_file_path)) {
        std::cerr << "Can't find specified config file, please double-check path" << std::endl;
        return 1;
    }

    sim::Config config;
    std::cout << "Loading config... ";
    if (auto config_opt = sim::Config::load(config_file_path)) {
        std::cout << "Config loaded successfully" << std::endl;
        config = config_opt.value();
    } else {
        std::cerr << "Failed to load config" << std::endl;
        return 1;
    }

    common::CoordinateSystem coordinate_system(config.origin);
    sim::Simulation simulation(config);
    sim::HALProviderSimImpl sim_hal(&simulation, coordinate_system);
    events::EventQueue event_queue;

    comms::Comms comms(config.control_server_url, events::RouteControl(&event_queue), events::ErrorReporting(&event_queue), sim_hal.positioning());

    robot::Robot robot = robot::Robot(&sim_hal, &event_queue);

    sim::Recording recording;
    recording.add_config(config);

    std::cout << "Starting simulation..." << std::endl;

    bool ok = comms.open();
    if (!ok) {
        std::cout << "Failed to open server comms" << std::endl;
        return -1;
    }

    std::vector<common::Coordinates> route;
    for (auto& waypoint : config.waypoints) {
        route.push_back(coordinate_system.project(waypoint));
    }

    std::cout << "Route prepared, beginning override" << std::endl;

    ok = comms.overrideRoute(route);
    if (!ok) {
        std::cout << "Failed to set route override" << std::endl;
        comms.close();
        return -1;
    }

    std::cout << "Route override set" << std::endl;

    // Allow server comms to start - small delays during simulation become much
    // longer in replay since sim time runs much faster
    // Also ensures simulation route overrides are in place
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    int current_iteration = 0;
    while (current_iteration++ < config.iteration_limit) {
        robot.update();
        sim_hal.update();
        simulation.update();

        auto [position, heading] = simulation.getPose();

        double distance_to_goal = (position - config.waypoints[config.waypoints.size() - 1]).magnitude();
        if (distance_to_goal <= config.end_distance) {
            break;
        }

        double left_speed = 0.5 * (sim_hal.motor_assembly()->front_left()->get_speed() + sim_hal.motor_assembly()->back_left()->get_speed());
        double right_speed = 0.5 * (sim_hal.motor_assembly()->front_right()->get_speed() + sim_hal.motor_assembly()->back_right()->get_speed());
        common::Vector2 motor_speed = common::Vector2(left_speed, right_speed);

        recording.add_entry(position, heading, motor_speed);
    }

    ok = comms.close();
    if (!ok) {
        std::cout << "Failed to set close comms" << std::endl;
        return -1;
    }

    std::cout << "Simulation complete" << std::endl;

    std::filesystem::path output_file_path = std::filesystem::path("sim_output.json");
    recording.write(output_file_path);

    std::cout << "Simulation output written to " << output_file_path << std::endl;

    return 0;
}
