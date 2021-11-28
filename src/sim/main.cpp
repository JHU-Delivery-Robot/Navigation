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

    physics.setPose(config.start_position, config.start_angle);

    // Initialize physics
    physics.update(0.0);

    robot::Robot robot = robot::Robot(&sim_hal);
    robot.updateGoal(config.goal_position);
    robot::GradientPotentialMap potential_map_parallel_copy = robot::GradientPotentialMap(600, 0.08, 1E5, config.goal_position);

    sim::Recording recording;
    recording.add_config(config);

    std::cout << "Starting simulation..." << std::endl;

    int current_iteration = 0;
    while (current_iteration++ < config.iteration_limit) {
        robot.update();

        auto [initial_position, initial_heading] = physics.getPose();

        potential_map_parallel_copy.updateLidarScan(sim_hal.lidar()->getLatestScan());
        auto lidar_position = initial_position + common::Vector2::polar(initial_heading, 25);
        auto attractive_gradient = potential_map_parallel_copy.getAttractivePotential(lidar_position);
        auto repulsive_gradient = potential_map_parallel_copy.getRepulsivePotential();

        physics.update(config.time_step);

        auto [position, heading] = physics.getPose();
        double distance_to_goal = (position - config.goal_position).magnitude();

        if (distance_to_goal <= config.end_distance) {
            break;
        }

        double left_speed = 0.5 * (sim_hal.motor_assembly()->front_left()->get_speed() + sim_hal.motor_assembly()->back_left()->get_speed());
        double right_speed = 0.5 * (sim_hal.motor_assembly()->front_right()->get_speed() + sim_hal.motor_assembly()->back_right()->get_speed());
        common::Vector2 motor_speed = common::Vector2(left_speed, right_speed);

        recording.add_entry(position, heading, motor_speed, attractive_gradient, repulsive_gradient);
    }

    recording.write("sim_output.json");

    std::cout << "Simulation finished successfully" << std::endl;
}
