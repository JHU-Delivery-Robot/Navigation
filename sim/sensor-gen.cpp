#include "sensor-gen.hpp"

#include <random>
#include <cmath>
#include <limits>

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

SensorGen::SensorGen(ObstMap& map, BeamModel& mdl) :
    map(map),
    beam(mdl) {}

// all sensors: x0 y0 center of sensor, range of angles in test
ConeSensor::ConeSensor(ObstMap& map, Angle fov, BeamModel& mdl) :
    SensorGen(map, mdl),
    fov(fov) {}

Length ConeSensor::generate(Length x, Length y, Angle heading) {
    Length distance = std::numeric_limits<double>::max();
    Length distanceTemp = 0.0;

    for (Angle iterator = heading - 0.5 * fov; heading <= heading + 0.5 * fov; heading += ANGLE_INCREMENT) {
        distanceTemp = map.distToObstacle(x, y, heading);
        if (distanceTemp < distance) {
            distance = distanceTemp;
        }
    }

    return distance;
}

Lidar::Lidar(ObstMap& map, BeamModel& mdl) :
    SensorGen(map, mdl) {}

void Lidar::generate(Length *readings, Length x, Length y) {
    for (Angle iterator = heading; heading <= 2 * PI; heading += ANGLE_INCREMENT) {
        *(readings++) = map.distToObstacle(x, y, heading);
    }
}

UltrasoundSensor::UltrasoundSensor(ObstMap& map, Angle fov, BeamModel& mdl) :
    ConeSensor(map, mdl, fov) {}

Length UltrasoundSensor::generate(Length x, Length y, Angle heading) {
    return ConeSensor::generate(x, y, heading);
}

IRSensor::IRSensor(ObstMap& map, Angle fov, BeamModel& mdl) :
    ConeSensor(map, mdl, fov) {}

Length IRSensor::generate(Length x, Length y, Angle heading) {
    return ConeSensor::generate(x, y, heading);
}