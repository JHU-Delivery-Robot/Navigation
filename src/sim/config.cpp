#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

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
    Config config;

    std::ifstream config_file = std::ifstream(config_file_path);
    if (!config_file.is_open()) {
        std::cerr << "Failed to open config file" << std::endl;
        return {};
    }

    nlohmann::json config_json;

    try {
        config_file >> config_json;
    } catch (const nlohmann::detail::parse_error& e) {
        std::cerr << "Failed to parse config file: " << e.what() << std::endl;
        return {};
    }

    if (config_json.contains("end_distance")) {
        config.end_distance = config_json["end_distance"].get<double>();
    }

    if (config_json.contains("time_step")) {
        config.time_step = config_json["time_step"].get<double>();
    }

    if (config_json.contains("iteration_limit")) {
        config.iteration_limit = config_json["iteration_limit"].get<int>();
    }

    if (config_json.contains("map_size")) {
        config.map_size = config_json["map_size"].get<double>();
    }

    if (config_json.contains("start_angle")) {
        config.start_angle = config_json["start_angle"].get<double>() * PI / 180.0;
        config.start_position = parse_point(config_json["start_position"]);
        config.waypoints = parse_point_list(config_json["waypoints"]);
    }

    if (config_json.contains("obstacles")) {
        for (auto&& points : config_json["obstacles"]) {
            auto polygon = sim::Polygon(parse_point_list(points));
            config.obstacles.push_back(polygon);
        }
    }

    return config;
}

}  // namespace sim
