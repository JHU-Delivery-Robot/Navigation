#ifndef OBSTACLE_MAP_HPP
#define OBSTACLE_MAP_HPP

#include <vector>
#include <tuple>

#include "vector2.hpp"
#include "polygon.hpp"

namespace sim {

class ObstacleMap {
public:
    ObstacleMap(std::vector<Polygon> obstacles);

    std::tuple<bool, double> distanceToObstacle(common::Vector2 position, common::Vector2 direction) const;

private:
    std::vector<Polygon> obstacles;
};

}

#endif
