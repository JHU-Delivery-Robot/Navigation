#include "recording.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace sim {

Recording::RobotState::RobotState() : position(0, 0), angle(0), motor_speeds(0, 0) {}
Recording::RobotState::RobotState(common::Vector2 position, double angle, common::Vector2 motor_speeds) : position(position), angle(angle), motor_speeds(motor_speeds) {}

void to_json(nlohmann::ordered_json& json, const Recording::RobotState& RobotState) {
    json["position"] = RobotState.position;
    json["angle"] = RobotState.angle;
    json["motor_speeds"] = RobotState.motor_speeds;
}

void from_json(const nlohmann::ordered_json& json, Recording::RobotState& RobotState) {
    RobotState.position = json.at("position").get<common::Vector2>();
    RobotState.angle = json.at("angle").get<double>();
    RobotState.motor_speeds = json.at("motor_speeds").get<common::Vector2>();
}

void Recording::add_config(sim::Config config) {
    this->config = config;
    add_entry(config.start_position, config.start_angle, common::Vector2(0, 0));
}

void Recording::add_entry(common::Vector2 robot_position, double robot_angle, common::Vector2 motor_speeds) {
    Recording::RobotState RobotState(robot_position, robot_angle, motor_speeds);
    replay_entries.push_back(RobotState);
}

nlohmann::ordered_json Recording::to_json() const {
    nlohmann::ordered_json json;

    json["config"] = config;
    json["replay"] = replay_entries;
    
    return json;
}

bool Recording::write(std::filesystem::path output_file_path) {
    std::ofstream output_file(output_file_path);
    if (!output_file.is_open() || !output_file.good()) {
        return false;
    }

    output_file << to_json() << std::endl;

    return true;
}

}  // namespace sim
