#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

namespace robot {

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

    Config config = config_json;
    if (config.control_server_url == "") {
        std::cerr << "Config file doesn't specify required field 'control_server_url'" << std::endl;
        return {};
    }

    if (config.root_ca_cert == "") {
        std::cerr << "Config file doesn't specify required field 'root_ca_cert'" << std::endl;
        return {};
    }

    if (config.robot_cert == "") {
        std::cerr << "Config file doesn't specify required field 'robot_cert'" << std::endl;
        return {};
    }

    if (config.robot_key == "") {
        std::cerr << "Config file doesn't specify required field 'robot_key'" << std::endl;
        return {};
    }

    if (config.lidar_port == "") {
        std::cerr << "Config file doesn't specify required field 'lidar_port'" << std::endl;
        return {};
    }

    return config_json;
}

void to_json(nlohmann::ordered_json& json, const Config& config) {
    json["control_server_url"] = config.control_server_url;
    json["root_ca_cert"] = config.root_ca_cert;
    json["robot_cert"] = config.robot_cert;
    json["robot_key"] = config.robot_key;
    json["lidar_port"] = config.lidar_port;
}

void from_json(const nlohmann::ordered_json& json, Config& config) {
    config.control_server_url = json.value("control_server_url", "");
    config.root_ca_cert = json.value("root_ca_cert", "");
    config.robot_cert = json.value("robot_cert", "");
    config.robot_key = json.value("robot_key", "");
    config.lidar_port = json.value("lidar_port", "");
}

}  // namespace sim
