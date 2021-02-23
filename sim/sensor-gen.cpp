#include "sensor-gen.hpp"

#include <random>
#include <cmath>

#include "obstacle_map.hpp"


BeamModel::BeamModel(double std_dev, double lambda, double max_reading,
                     double w_exp, double w_rand) :
    std_dev{std_dev},
    lambda{lambda},
    max_reading{max_reading},
    p_exp{w_exp},
    p_rand{w_rand} {
        // constructor body
        // need to initialize w_hit and w_max
        // w_hit = 1 - w_rand - w_exp; eliminate; only weigh prob of noise and glitching
        p_max = 1 - p_rand - p_exp;
}

Length BeamModel::sampleNormal(Length dist) {
    srand(1);
    // add dist to private class fields
    std::default_random_engine gen;
    std::mt19937 gen{rd()}; // exp_dist(gen) to sample single number for exp_dist
    std::exponential_distribution<double> exp_dist(lambda);
    std::normal_distribution<double> norm_dist(dist, std_dev);
    std::uniform_real_distribution<double> uniform_dist(0,max_reading);
    
    double rand_exp = exp_dist(gen);
    double rand_norm = norm_dist(gen);
    double rand_uniform = uniform_dist(gen);
    return  p_exp*rand_exp + p_hit*rand_norm + p_max*max_reading + p_rand*rand_uniform;
}

SensorGen::SensorGen(ObstMap& map) :
    map(map) {}

SensorGen::SensorGen(ObstMap& map, BeamModel& mdl) :
    map(map),
    beam(mdl) {}

// all sensors: x0 y0 center of sensor, range of angles in test
Lidar::Lidar(ObstMap& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}

void Lidar::generate(Length *readings, Length x, Length y) {
    *readings = sqrt(pow(x,2) + pow(y,2));
}

UltrasoundSensor::UltrasoundSensor(ObstMap& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}

Length UltrasoundSensor::generate(Length x, Length y, Angle fov, Angle heading) {
    return cos(heading) * sqrt(pow(x,2)/pow(sin(fov),2)-pow(y,2));
}

IRSensor::IRSensor(ObstMap& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}
