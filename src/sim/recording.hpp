#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.hpp"
#include "polygon.hpp"
#include "vector2.hpp"

namespace sim {

class Recording {
public:
    Recording(std::filesystem::path output_file_path);
    ~Recording();

    bool ok();

    void write_config(sim::Config config);
    void write(common::Vector2 robot_position, double robot_angle, common::Vector2 motor_speed,
               common::Vector2 attractive_gradient, common::Vector2 repulsive_gradient);

private:
    std::ofstream output_file;
};

}  // namespace sim

#endif
