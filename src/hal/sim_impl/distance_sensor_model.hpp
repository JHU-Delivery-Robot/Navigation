#ifndef DISTANCE_SENSOR_MODEL_HPP
#define DISTANCE_SENSOR_MODEL_HPP

#include "beam_model.hpp"
#include "sim/obstacle_map.hpp"
#include "common/vector2.hpp"

namespace sim {

class DistanceSensorModel
{
public:
    DistanceSensorModel(const ObstacleMap *map, BeamModel beam_model, double max_range);

    double sample(common::Vector2 position, common::Vector2 direction);

private:
    BeamModel beam_model;
    const ObstacleMap* map;
    const double max_range;
};

}

#endif
