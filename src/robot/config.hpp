#ifndef ROBOT_CONFIG_HPP
#define ROBOT_CONFIG_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

namespace robot {

class Config {
public:
    static std::optional<Config> load(const std::filesystem::path& config_file_path);

    friend void to_json(nlohmann::ordered_json& json, const Config& config);
    friend void from_json(const nlohmann::ordered_json& json, Config& config);

    std::string control_server_url;
    
    std::string root_ca_cert;
    std::string robot_cert;
    std::string robot_key;

    std::string lidar_port;
};

void to_json(nlohmann::ordered_json& json, const Config& config);
void from_json(const nlohmann::ordered_json& json, Config& config);

}  // namespace robot

#endif
