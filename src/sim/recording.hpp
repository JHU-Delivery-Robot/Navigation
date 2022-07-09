#ifndef RECORDING_HPP
#define RECORDING_HPP

#include <filesystem>
#include <map>

#include "config.hpp"
#include "polygon.hpp"
#include "common/vector2.hpp"
#include "nlohmann/json.hpp"

namespace sim {

class Recording {
public:
    void add_config(sim::Config config);
    void add_entry(common::Vector2 robot_position, double robot_angle, common::Vector2 motor_speed);

    bool write(std::filesystem::path output_file_path);

private:
    class RobotState {
    public:
        RobotState();
        RobotState(common::Vector2 position, double angle, common::Vector2 motor_speeds);

        friend void to_json(nlohmann::ordered_json& json, const RobotState& entry);
        friend void from_json(const nlohmann::ordered_json& json, RobotState& entry);

        common::Vector2 position;
        double angle;
        common::Vector2 motor_speeds;
    };

    nlohmann::ordered_json to_json() const;

    friend void to_json(nlohmann::ordered_json& json, const RobotState& entry);
    friend void from_json(const nlohmann::ordered_json& json, RobotState& entry);

    Config config;
    std::vector<RobotState> replay_entries;
};

void to_json(nlohmann::ordered_json& json, const Recording::RobotState& entry);
void from_json(const nlohmann::ordered_json& json, Recording::RobotState& entry);

}  // namespace sim

#endif
