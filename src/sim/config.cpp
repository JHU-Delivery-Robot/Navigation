#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

namespace sim {

std::string trim(std::string s) {
    auto left_it = std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); });
    s.erase(s.begin(), left_it);

    auto right_it = std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); });
    s.erase(right_it.base(), s.end());

    return s;
}

common::Vector2 parse_point(std::string s) {
    std::string x_str = s.substr(s.find('(') + 1, s.find(','));
    std::string y_str = s.substr(s.find(',') + 1, s.find(')'));

    double x = std::stod(x_str);
    double y = std::stod(y_str);

    return common::Vector2(x, y);
}

std::vector<common::Vector2> parse_point_list(std::string s) {
    std::size_t last = 0;
    std::size_t next = 0;

    std::vector<common::Vector2> points;

    while ((next = s.find(')', last)) != std::string::npos) {
        common::Vector2 point = parse_point(s.substr(last, next - last));
        points.push_back(point);

        if (s.find(',', next) == std::string::npos) {
            break;
        }

        last = s.find(',', next) + 1;
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

    std::string line;
    while (std::getline(config_file, line)) {
        std::size_t comma_index = line.find(',');
        if (comma_index == std::string::npos) {
            continue;
        }

        std::string key = trim(line.substr(0, comma_index));
        std::string value = trim(line.substr(comma_index + 1));

        if (key == "end_distance") {
            config.end_distance = std::stod(value);
        } else if (key == "time_step") {
            config.time_step = std::stod(value);
        } else if (key == "iteration_limit") {
            config.iteration_limit = std::stoi(value);
        } else if (key == "map_size") {
            config.map_size = std::stoi(value);
        } else if (key == "start_angle") {
            config.start_angle = std::stod(value) * PI / 180.0;
        } else if (key == "start_position") {
            config.start_position = parse_point(value);
        } else if (key == "waypoints") {
            config.waypoints = parse_point_list(value);
        } else if (key == "obstacles") {
            auto points = parse_point_list(value);
            auto polygon = sim::Polygon(points);
            config.obstacles.push_back(polygon);
        } else if (key == "control_server_url") {
            config.control_server_url = trim(value);
        } else {
            std::cerr << "Unrecognized sim config key: " << key << std::endl;
            return {};
        }
    }

    return config;
}

}  // namespace sim
