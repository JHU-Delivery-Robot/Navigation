#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace sim {

common::Vector2 parse_point(nlohmann::json point_json) {
    return common::Vector2(point_json[0].get<double>(), point_json[1].get<double>());
}

std::vector<common::Vector2> parse_point_list(nlohmann::json points_json) {
    std::vector<common::Vector2> points;

    for (auto&& point : points_json) {
        points.push_back(parse_point(point));
    }

    return points;
}

std::optional<Config> Config::load(const std::filesystem::path& config_file_path) {
    std::ifstream config_file = std::ifstream(config_file_path);
    if (!config_file.is_open()) {
        std::cerr << "Failed to open config file" << std::endl;
        return {};
    }

    nlohmann::ordered_json config_json;

    try {
        config_file >> config_json;
    } catch (const nlohmann::detail::parse_error& e) {
        std::cerr << "Failed to parse config file: " << e.what() << std::endl;
        return {};
    }

    return config_json;
}

void to_json(nlohmann::ordered_json& json, const Config& config) {
    json["end_distance"] = config.end_distance;
    json["time_step"] = config.time_step;
    json["iteration_limit"] = config.iteration_limit;
    json["map_size"] = config.map_size;
    json["start_angle"] = config.start_angle;
    json["start_positionconfig."] = config.start_position;
    json["waypoints"] = config.waypoints;
    json["obstacles"] = config.obstacles;
}

void from_json(const nlohmann::ordered_json& json, Config& config) {
    config.end_distance = json.value("end_distance", config.end_distance);
    config.time_step = json.value("time_step", config.time_step);
    config.iteration_limit = json.value("iteration_limit", config.iteration_limit);
    config.map_size = json.value("map_size", config.map_size);
    config.start_angle = json.value("start_angle", config.start_angle * 180.0 / PI) * PI / 180.0;
    config.start_position = json.value("start_position", config.start_position);
    config.waypoints = json.value("waypoints", config.waypoints);
    config.obstacles = json.value("obstacles", config.obstacles);
}

}  // namespace sim
