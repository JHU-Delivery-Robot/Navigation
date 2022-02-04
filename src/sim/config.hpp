#ifndef SIM_CONFIG_HPP
#define SIM_CONFIG_HPP

#include <filesystem>
#include <optional>
#include <vector>

#include "common.hpp"
#include "polygon.hpp"
#include "vector2.hpp"

namespace sim {

class Config {
public:
    static std::optional<Config> load(const std::filesystem::path& config_file_path);

    double end_distance = 20;
    double time_step = 0.02;
    int iteration_limit = 60 * 50;
    int map_size = 800;

    common::Vector2 start_position = common::Vector2(-300.0, -200.0);
    double start_angle = 0.5 * PI;
    std::vector<common::Vector2> waypoints = {common::Vector2(350.0, 300.0)};

    std::vector<sim::Polygon> obstacles;
};

}  // namespace sim

#endif
