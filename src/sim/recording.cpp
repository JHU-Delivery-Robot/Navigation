#include "recording.hpp"

#include <fstream>
#include <iostream>

namespace sim {

void Recording::add_config(sim::Config config) {
    data["size"] = std::to_string(config.map_size);
    data["time_step"] = std::to_string(config.time_step);
    data["goal"] = "[" + std::to_string(config.goal_position.x) + ", " + std::to_string(config.goal_position.y) + "]";

    std::stringstream ss;

    ss << "[";

    for (std::size_t i = 0; i < config.obstacles.size(); i++) {
        ss << config.obstacles[i];

        if (i < config.obstacles.size() - 1) {
            ss << ", ";
        }
    }

    ss << "]";
    data["obstacles"] = ss.str();

    add_entry(config.start_position, config.start_angle, common::Vector2(0, 0), common::Vector2(0, 0), common::Vector2(0, 0));
}

void Recording::add_entry(common::Vector2 robot_position, double robot_angle, common::Vector2 motor_speed,
                          common::Vector2 attractive_gradient, common::Vector2 repulsive_gradient) {
    replay_entries.push_back({robot_position, robot_angle, motor_speed, attractive_gradient, repulsive_gradient});
}

std::string Recording::serialize_entries() {
    std::stringstream ss;
    ss << "[";

    for (std::size_t i = 0; i < replay_entries.size(); i++) {
        const auto& [robot_position, robot_angle, motor_speed, attractive_gradient, repulsive_gradient] = replay_entries[i];

        ss << "[" << robot_position.x << ", " << robot_position.y << ", " << robot_angle << ", "
           << motor_speed.x << ", " << motor_speed.y << ", "
           << attractive_gradient.x << ", " << attractive_gradient.y << ", "
           << repulsive_gradient.x << ", " << repulsive_gradient.y << "]";

        if (i < replay_entries.size() - 1) {
            ss << ", \n";
        }
    }

    ss << "]";
    return ss.str();
}

bool Recording::write(std::filesystem::path output_file_path) {
    std::ofstream output_file(output_file_path);
    if (!output_file.is_open() || !output_file.good()) {
        return false;
    }

    output_file << "{\n";

    for (const auto& [key, value] : data) {
        output_file << R"(")" << key << R"(": )" << value << ",\n";
    }

    output_file << R"("positions": )" << serialize_entries() << "\n";

    output_file << "}\n";

    return true;
}

}  // namespace sim
