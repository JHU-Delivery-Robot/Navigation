#include "recording.hpp"

namespace sim {

Recording::Recording(std::filesystem::path output_file_path) : output_file(output_file_path) {}

Recording::~Recording() {
    output_file << "]}" << std::endl;
    output_file.close();
}

bool Recording::ok() {
    return output_file.is_open() && output_file.good();
}

void Recording::write_config(sim::Config config) {
    if (!ok()) {
        return;
    }

    output_file << R"({"size": )" << config.map_size << ", "
                << R"("time_step": )" << config.time_step << ", "
                << R"("goal": [)" << config.goal_position.x << ", " << config.goal_position.y << "], "
                << R"("obstacles": [)";

    for (size_t i = 0; i < config.obstacles.size(); i++) {
        output_file << config.obstacles[i];

        if (i < config.obstacles.size() - 1) {
            output_file << ", ";
        }
    }

    output_file << R"(], "positions" : [ [)" << config.start_position.x << ", " << config.start_position.y
                << ", " << config.start_angle << ", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ]";
}

void Recording::write(common::Vector2 robot_position, double robot_angle, common::Vector2 motor_speed,
                      common::Vector2 attractive_gradient, common::Vector2 repulsive_gradient) {
    if (!ok()) {
        return;
    }

    output_file << ",\n[ " << robot_position.x << ", " << robot_position.y << ", " << robot_angle << ", "
                << motor_speed.x << ", " << motor_speed.y << ", "
                << attractive_gradient.x << ", " << attractive_gradient.y << ", "
                << repulsive_gradient.x << ", " << repulsive_gradient.y << " ]";
}

}  // namespace sim
