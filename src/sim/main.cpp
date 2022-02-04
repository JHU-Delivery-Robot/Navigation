#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>

#include "config.hpp"
#include "hal_provider_sim_impl.hpp"
#include "obstacle_map.hpp"
#include "physics_sim.hpp"
#include "polygon.hpp"
#include "recording.hpp"
#include "robot.hpp"

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
        config = config_opt.value();
        std::cout << "Config loaded successfully" << std::endl;
    } else {
        std::cerr << "Failed to load config" << std::endl;
        return 1;
    }

    sim::HALProviderSimImpl sim_hal(config.obstacles);
    sim::PhysicsSim physics = sim::PhysicsSim(&sim_hal);

    // Initialize physics
    physics.setPose(config.start_position, config.start_angle);
    sim_hal.updatePose(config.start_position, config.start_angle);
    sim_hal.motor_assembly()->reset_odometry();
    physics.update();

    robot::Robot robot = robot::Robot(&sim_hal);
    robot.setWaypoints(config.waypoints);

    sim::Recording recording;
    recording.add_config(config);

    std::cout << "Starting simulation..." << std::endl;

    int current_iteration = 0;
    while (current_iteration++ < config.iteration_limit) {
        robot.update();
        
        sim_hal.motor_assembly()->update(config.time_step);
        physics.update();
        auto [position, heading] = physics.getPose();
        sim_hal.updatePose(position, heading);

        double distance_to_goal = (position - config.waypoints[config.waypoints.size() - 1]).magnitude();
        if (distance_to_goal <= config.end_distance) {
            break;
        }

        double left_speed = 0.5 * (sim_hal.motor_assembly()->front_left()->get_speed() + sim_hal.motor_assembly()->back_left()->get_speed());
        double right_speed = 0.5 * (sim_hal.motor_assembly()->front_right()->get_speed() + sim_hal.motor_assembly()->back_right()->get_speed());
        common::Vector2 motor_speed = common::Vector2(left_speed, right_speed);

        recording.add_entry(position, heading, motor_speed);
    }

    std::filesystem::path output_file_path = std::filesystem::path("sim_output.json");
    recording.write(output_file_path);

    std::cout << "Simulation finished successfully, output written to " << output_file_path << std::endl;
}
