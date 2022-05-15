#ifndef SIM_CONFIG_HPP
#define SIM_CONFIG_HPP

#include <filesystem>
#include <optional>
#include <vector>

#include "common/common.hpp"
#include "common/coordinates.hpp"
#include "common/vector2.hpp"
#include "polygon.hpp"

namespace sim {

class Config {
public:
    static std::optional<Config> load(const std::filesystem::path& config_file_path);

    std::string control_server_url = "127.0.0.1:9000";

    double end_distance = 0.2;
    double time_step = 0.02;
    int iteration_limit = 60 * 50;
    double map_size = 0.8;

    common::Coordinates origin = common::Coordinates(39.3289, -76.6215);
    common::Vector2 start_position = common::Vector2(-3.0, -2.0);
    double start_angle = 0.5 * PI;
    std::vector<common::Vector2> waypoints = {common::Vector2(3.5, 3.0)};

    std::vector<sim::Polygon> obstacles;
};

}  // namespace sim

#endif
