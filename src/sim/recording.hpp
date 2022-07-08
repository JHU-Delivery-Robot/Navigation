#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <filesystem>
#include <map>

#include "config.hpp"
#include "polygon.hpp"
#include "common/vector2.hpp"

namespace sim {

class Recording {
public:
    void add_config(sim::Config config);
    void add_entry(common::Vector2 robot_position, double robot_angle, common::Vector2 motor_speed);

    bool write(std::filesystem::path output_file_path);

private:
    std::map<std::string, std::string> data;
    std::vector<std::tuple<common::Vector2, double, common::Vector2>> replay_entries;

    std::string serialize_entries();
};

}  // namespace sim

#endif
