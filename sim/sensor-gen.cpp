#include "sensor-gen.hpp"

#include <cmath>
#include <limits>

#include "obstacle_map.hpp"

BeamModel::BeamModel(double std_dev, double lambda, double max_reading,
                     double w_exp, double w_rand) :
    std_dev{std_dev},
    lambda{lambda},
    max_reading{max_reading},
    p_exp{w_exp},
    p_rand{w_rand},
    exp_dist(lambda),
    uniform_dist_rand(0,1) {
        std::random_device rd;
        gen.seed(rd());
        
}

Length BeamModel::sampleNormal(Length dist) {
    std::normal_distribution<double> norm_dist(dist, std_dev);
    Length rand_norm = norm_dist(gen);
    
    if (rand_norm < max_reading) {
        return rand_norm;
    } else {
        return max_reading;
    }
}

Length BeamModel::sampleGlitch() {
    if (uniform_dist_rand(gen)*(p_exp + p_rand) < p_exp) {
        return exp_dist(gen);
    } else {
        return uniform_dist_rand(gen)*max_reading;
    } 
}

bool BeamModel::glitch() {
    return uniform_dist_rand(gen) < (p_exp + p_rand);
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