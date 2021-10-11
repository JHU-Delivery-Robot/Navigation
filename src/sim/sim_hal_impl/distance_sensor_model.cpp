#include "distance_sensor_model.hpp"

namespace sim {

DistanceSensorModel::DistanceSensorModel(ObstacleMap* map, BeamModel beam_model, double max_range) :
    beam_model(beam_model),
    map(map),
    max_range(max_range) {}

double DistanceSensorModel::sample(common::Vector2 position, common::Vector2 direction) {
    auto [intersection_found, distance] = map->distanceToObstacle(position, direction);

    if (intersection_found && distance < max_range) {
        return beam_model.sample(distance);
    } else {
        return max_range;
    }
}

}
