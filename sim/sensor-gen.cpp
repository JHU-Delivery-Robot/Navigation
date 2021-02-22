#include "sensor-gen.hpp"

#include "obstacle_map.hpp"
#include <random.h>
#include <math.h>

BeamModel::BeamModel(const Obst_Map& map, double std_dev, double lambda, double max_reading,
                     double w_exp, double w_rand) :
    std_dev{std_dev},
    lambda{lambda},
    max_reading{max_reading},
    w_exp{w_exp},
    w_rand{w_rand} {
        // constructor body
        // need to initialize w_hit and w_max
        // w_hit = 1 - w_rand - w_exp;
        w_max = 1 - w_hit - w_rand - w_exp;
}

Length BeamModel::sample(Length dist) {
    srand(1);
    std::default_random_engine gen;
    std::exponential_distribution<double> exp_dist(lambda);
    std::normal_distribution<double> norm_dist(mean, std_dev); // need mean in param?
    std::uniform_real_distribution<double> uniform_dist(0,max_reading);
    
    double rand_exp = exp_dist(gen);
    double rand_norm = norm_dist(gen);
    double rand_uniform = uniform_dist(gen);
    return  w_exp*rand_exp + w_hit*rand_norm + w_max*max_reading + w_rand*rand_uniform;
}

SensorGen::SensorGen(Obst_Map& map) :
    map(map) {}

SensorGen::SensorGen(Obst_Map& map, BeamModel& mdl) :
    map(map),
    beam(mdl) {}

Lidar::Lidar(Obst_Map& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}

void Lidar::generate(Length *readings, Length x, Length y) {
    *readings = sqrt(pow(x,2) + pow(y,2));
}

UltrasoundSensor::UltrasoundSensor(Obst_Map& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}

Length UltrasoundSensor::generate(Length x, Length y, Angle fov, Angle heading) {
    return cos(heading) * sqrt(pow(x,2)/pow(sin(fov),2)-pow(y,2));
}

IRSensor::IRSensor(Obst_Map& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}
