#include <iomanip>
#include <iostream>
#include <fstream>

#include "hal_provider_sim_impl.hpp"
#include "obstacle_map.hpp"
#include "physics_sim.hpp"
#include "polygon.hpp"
#include "robot.hpp"

int main () {
    sim::Polygon obstacle1 = sim::Polygon({ common::Vector2(150, 100), common::Vector2(400, 100), common::Vector2(400, -100), common::Vector2(150, -100) });
    sim::Polygon obstacle2 = sim::Polygon({ common::Vector2(-150, 100), common::Vector2(-400, 100), common::Vector2(-400, -100), common::Vector2(-150, -100) });

    sim::HALProviderSimImpl sim_hal({ obstacle1, obstacle2 });
    sim::PhysicsSim physics = sim::PhysicsSim(&sim_hal);

    common::Vector2 start = common::Vector2(-300.0, -200.0);
    common::Vector2 goal = common::Vector2(350.0, 300.0);

    physics.setPose(start, 0.5*PI);

    robot::Robot robot = robot::Robot(&sim_hal);

    constexpr double min_distance = 20;
    constexpr double time_step = 0.02; // 50 Hz
    constexpr double time_limit = 60;

    std::ofstream sim_output_file("sim_output.json");
    if (!sim_output_file.is_open())
    {
        std::cout << "Couldn't open log file";
        return 1;
    }

    sim_output_file << R"({"size": 800, "time_step": 0.02, "goal": [350.0, 300.0], "obstacles": [)"
                  << R"([ [ 150, 100 ], [ 400, 100 ], [ 400, -100 ], [ 150, -100 ] ],)"
                  << R"([ [ -150, 100 ], [ -400, 100 ], [ -400, -100 ], [ -150, -100 ] ] ],)"
                  << R"("positions": [ [-300.0, -200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] )";

    robot::GradientPotentialMap potential_map_parallel_copy = robot::GradientPotentialMap(600, 0.08,  1E5, goal);
    physics.update(0.0); // initialize physics

    double time = 0.0;
    while (time < time_limit) {
        robot.update();

        auto [initial_position, initial_heading] = physics.getPose();

        potential_map_parallel_copy.updateLidar(sim_hal.lidar()->read(), initial_heading);
        auto lidar_position = initial_position + common::Vector2::polar(initial_heading, 25);
        auto attractiveGradient = potential_map_parallel_copy.getAttractivePotential(lidar_position);
        auto repulsiveGradient = potential_map_parallel_copy.getRepulsivePotential();

        physics.update(time_step);
        time += time_step;

        auto [position, heading] = physics.getPose();
        double distance_to_goal = (position - goal).magnitude();

        if (distance_to_goal <= min_distance) {
            break;
        }

        double left_speed = 0.5*(sim_hal.motor_assembly()->front_left()->get_speed() + sim_hal.motor_assembly()->back_left()->get_speed());
        double right_speed = 0.5*(sim_hal.motor_assembly()->front_right()->get_speed() + sim_hal.motor_assembly()->back_right()->get_speed());

        sim_output_file << ",\n[ " << position.x << ", " << position.y << ", " << heading << ", " << left_speed << ", " << right_speed << ", " << attractiveGradient.x << ", " << attractiveGradient.y << ", " << repulsiveGradient.x << ", " << repulsiveGradient.y << " ]";
    }

    sim_output_file << "]}" << std::endl;
    sim_output_file.close();
}
