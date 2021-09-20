#include "sensor-gen.hpp"

#include <cmath>
#include <limits>

#include "obstacle_map.hpp"

namespace sim {

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

BeamModel::BeamModel(double std_dev, double lambda, double max_reading,
                     double w_exp, double w_rand, unsigned long seed) :
    std_dev{std_dev},
    lambda{lambda},
    max_reading{max_reading},
    p_exp{w_exp},
    p_rand{w_rand},
    exp_dist(lambda),
    uniform_dist_rand(0,1) {
        std::random_device rd;
        gen.seed(seed);
        
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

SensorGen::SensorGen(ObstacleMap* map, BeamModel* mdl, Length max) :
    beam(mdl),
    map(map),
    range(max) {}

// all sensors: x0 y0 center of sensor, range of angles in test
ConeSensor::ConeSensor(ObstacleMap* map, BeamModel* mdl, Length max, Angle fov) :
    SensorGen(map, mdl, max),
    fov(fov) {}

Length ConeSensor::generate(Length x, Length y, Angle heading) {
    Length distance = std::numeric_limits<double>::max();

    for (Angle iterator = heading - 0.5 * fov; iterator <= heading + 0.5 * fov; iterator += ANGLE_INCREMENT) {
        Length distanceCandidate = 0.0;

        if (beam->glitch()) {
            distanceCandidate = beam->sampleGlitch();
        } else {
            common::Vector2 position(x, y);
            common::Vector2 heading = common::Vector2::polar(iterator, 1.0);
            bool obstacle_found;

            std::tie(obstacle_found, distanceCandidate) = map->distanceToObstacle(position, heading);

            if (obstacle_found && distanceCandidate < range) {
                distanceCandidate = beam->sampleNormal(distanceCandidate);
            }
        }

        if (distanceCandidate < distance) {
            distance = distanceCandidate;
        }
    }

    return distance;
}

Lidar::Lidar(ObstacleMap* map, BeamModel* mdl, Length max) :
    SensorGen(map, mdl, max) {}

void Lidar::generate(Length *readings, Length x, Length y) {
    for (Angle iterator = 0.0; iterator <= 2 * PI; iterator += ANGLE_INCREMENT) {
        if (beam->glitch()) {
            *(readings++) = beam->sampleGlitch();
        } else {
            common::Vector2 position(x, y);
            common::Vector2 heading = common::Vector2::polar(iterator, 1.0);

            auto [ obstacle_found, distanceCandidate ] = map->distanceToObstacle(position, heading);

            if (distanceCandidate != range) {
                *(readings++) = beam->sampleNormal(distanceCandidate);
            } else {
                *(readings++) = distanceCandidate;
            }
        }
    }
}

UltrasoundSensor::UltrasoundSensor(ObstacleMap* map, BeamModel* mdl, Length max, Angle fov) :
    ConeSensor(map, mdl, max, fov) {}

Length UltrasoundSensor::generate(Length x, Length y, Angle heading) {
    return ConeSensor::generate(x, y, heading);
}

IRSensor::IRSensor(ObstacleMap* map, BeamModel* mdl, Length max, Angle fov) :
    ConeSensor(map, mdl, max, fov) {}

Length IRSensor::generate(Length x, Length y, Angle heading) {
    return ConeSensor::generate(x, y, heading);
}

}
