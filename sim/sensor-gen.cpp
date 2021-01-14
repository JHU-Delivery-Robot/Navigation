#include "sensor-gen.hpp"

#include "obstacle_map.hpp"

BeamModel::BeamModel(const Obst_Map& map, double std_dev, double lambda, double max_reading,
                     double w_exp, double w_rand) :
    std_dev{std_dev},
    lambda{lambda},
    max_reading{max_reading},
    w_exp{w_exp},
    w_rand{w_rand} {
        // constructor body
        // need to initialize
        // w_hit
        // w_max
}

SensorGen::SensorGen(Obst_Map& map, BeamModel& mdl) :
    map(map),
    beam(mdl) {}

Lidar::Lidar(Obst_Map& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}

UltrasoundSensor::UltrasoundSensor(Obst_Map& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}

IRSensor::IRSensor(Obst_Map& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}
