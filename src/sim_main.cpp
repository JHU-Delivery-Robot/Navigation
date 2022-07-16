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
#include "robot/config.hpp"
#include "robot/robot.hpp"
#include "sim/config.hpp"
#include "sim/obstacle_map.hpp"
#include "sim/polygon.hpp"
#include "sim/recording.hpp"
#include "sim/simulation.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <robot config> <sim config>" << std::endl;
        return 1;
    }

    std::cout << "Loading configs... \n";
    robot::Config robot_config;
    if (auto config_opt = robot::Config::load(argv[1])) {
        std::cout << "    Robot config loaded successfully" << std::endl;
        robot_config = config_opt.value();
    } else {
        std::cerr << "Failed to load robot config" << std::endl;
        return 1;
    }

    sim::Config sim_config;
    if (auto config_opt = sim::Config::load(argv[2])) {
        std::cout << "    Sim config loaded successfully" << std::endl;
        sim_config = config_opt.value();
    } else {
        std::cerr << "Failed to load sim config" << std::endl;
        return 1;
    }

    common::CoordinateSystem coordinate_system(sim_config.origin);
    sim::Simulation simulation(sim_config);
    sim::HALProviderSimImpl sim_hal(&simulation, coordinate_system);
    events::EventQueue event_queue;

    comms::Comms::Credentials server_credentials(robot_config.root_ca_cert, robot_config.robot_cert, robot_config.robot_key);
    comms::Comms comms(sim_config.control_server_url, server_credentials, events::RouteControl(&event_queue), events::ErrorReporting(&event_queue), sim_hal.positioning());

    robot::Robot robot = robot::Robot(&sim_hal, &event_queue);

    sim::Recording recording;
    recording.add_config(sim_config);

    std::cout << "Starting simulation..." << std::endl;

    bool ok = comms.open();
    if (!ok) {
        std::cout << "Failed to open server comms" << std::endl;
        return -1;
    }

    std::vector<common::Coordinates> route;
    for (auto& waypoint : sim_config.waypoints) {
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
    while (current_iteration++ < sim_config.iteration_limit) {
        robot.update();
        sim_hal.update();
        simulation.update();

        auto [position, heading] = simulation.getPose();

        double distance_to_goal = (position - sim_config.waypoints[sim_config.waypoints.size() - 1]).magnitude();
        if (distance_to_goal <= sim_config.end_distance) {
            break;
        }

        double left_speed = 0.5 * (sim_hal.motor_assembly()->front_left()->get_speed() + sim_hal.motor_assembly()->back_left()->get_speed());
        double right_speed = 0.5 * (sim_hal.motor_assembly()->front_right()->get_speed() + sim_hal.motor_assembly()->back_right()->get_speed());
        common::Vector2 motor_speed = common::Vector2(left_speed, right_speed);

        recording.add_entry(position, heading, motor_speed);
    }

    ok = comms.close();
    if (!ok) {
        std::cout << "Failed to close comms" << std::endl;
        return -1;
    }

    std::cout << "Simulation complete" << std::endl;

    std::filesystem::path output_file_path = std::filesystem::path("sim_output.json");
    recording.write(output_file_path);

    std::cout << "Simulation output written to " << output_file_path << std::endl;

    return 0;
}
