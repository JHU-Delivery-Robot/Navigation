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
#include "robot.hpp"

int main(int argc, char *argv[]) {
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

    robot::Robot robot = robot::Robot(&sim_hal);

    std::ofstream sim_output_file("sim_output.json");
    if (!sim_output_file.is_open()) {
        std::cout << "Couldn't open log file";
        return 1;
    }

    sim_output_file << R"({"size": 800, "time_step": 0.02, "goal": [350.0, 300.0], "obstacles": [)"
                    << R"([ [ 150, 100 ], [ 400, 100 ], [ 400, -100 ], [ 150, -100 ] ],)"
                    << R"([ [ -150, 100 ], [ -400, 100 ], [ -400, -100 ], [ -150, -100 ] ] ],)"
                    << R"("positions": [ [-300.0, -200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] )";

    robot::GradientPotentialMap potential_map_parallel_copy = robot::GradientPotentialMap(config.qStar, config.attractive_gradient_scale, config.repulsive_gradient_scale, config.goal_position);

    // Initialize physics
    physics.update(0.0);

    int current_iteration = 0;
    while (current_iteration++ < config.iteration_limit) {
        robot.update();

        auto [initial_position, initial_heading] = physics.getPose();

        potential_map_parallel_copy.updateLidar(sim_hal.lidar()->read(), initial_heading);
        auto lidar_position = initial_position + common::Vector2::polar(initial_heading, 25);
        auto attractiveGradient = potential_map_parallel_copy.getAttractivePotential(lidar_position);
        auto repulsiveGradient = potential_map_parallel_copy.getRepulsivePotential();

        physics.update(config.time_step);

        auto [position, heading] = physics.getPose();
        double distance_to_goal = (position - config.goal_position).magnitude();

        if (distance_to_goal <= config.end_distance) {
            break;
        }

        double left_speed = 0.5 * (sim_hal.motor_assembly()->front_left()->get_speed() + sim_hal.motor_assembly()->back_left()->get_speed());
        double right_speed = 0.5 * (sim_hal.motor_assembly()->front_right()->get_speed() + sim_hal.motor_assembly()->back_right()->get_speed());

        sim_output_file << ",\n[ " << position.x << ", " << position.y << ", " << heading << ", " << left_speed << ", " << right_speed << ", " << attractiveGradient.x << ", " << attractiveGradient.y << ", " << repulsiveGradient.x << ", " << repulsiveGradient.y << " ]";
    }

    sim_output_file << "]}" << std::endl;
    sim_output_file.close();
}
