#include "recording.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace sim {

void Recording::add_config(sim::Config config) {
    data["size"] = std::to_string(config.map_size);
    data["time_step"] = std::to_string(config.time_step);

    std::stringstream waypoints_ss;
    waypoints_ss << "[";

    for (std::size_t i = 0; i < config.waypoints.size(); i++) {
        auto waypoint = config.waypoints[i];
        waypoints_ss << "[" << std::to_string(waypoint.x) << ", " << std::to_string(waypoint.y) << "]";

        if (i < config.waypoints.size() - 1) {
            waypoints_ss << ", ";
        }
    }

    waypoints_ss << "]";
    data["waypoints"] = waypoints_ss.str();

    std::stringstream obstacles_ss;
    obstacles_ss << "[";

    for (std::size_t i = 0; i < config.obstacles.size(); i++) {
        obstacles_ss << config.obstacles[i];

        if (i < config.obstacles.size() - 1) {
            obstacles_ss << ", ";
        }
    }

    obstacles_ss << "]";
    data["obstacles"] = obstacles_ss.str();

    add_entry(config.start_position, config.start_angle, common::Vector2(0, 0));
}

void Recording::add_entry(common::Vector2 robot_position, double robot_angle, common::Vector2 motor_speed) {
    replay_entries.push_back({robot_position, robot_angle, motor_speed});
}

std::string Recording::serialize_entries() {
    std::stringstream ss;
    ss << "[";

    for (std::size_t i = 0; i < replay_entries.size(); i++) {
        const auto& [robot_position, robot_angle, motor_speed] = replay_entries[i];

        ss << "[" << robot_position.x << ", " << robot_position.y << ", " << robot_angle << ", "
           << motor_speed.x << ", " << motor_speed.y << "]";

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
