#include "obstacle_map.hpp"

#include <cmath>
#include <limits>

namespace sim {

ObstacleMap::ObstacleMap(std::vector<Polygon> obstacles) : obstacles(obstacles) {}

std::tuple<bool, double> ObstacleMap::distanceToObstacle(common::Vector2 position, common::Vector2 direction) const
{
    bool intersection_found = false;
    double shortest_distance = std::numeric_limits<double>::max();

    for (auto &&obstacle : obstacles)
    {
        auto [intersected, distance] = obstacle.intersect(position, direction);

        if (intersected && distance < shortest_distance)
        {
            shortest_distance = distance;
            intersection_found = true;
        }
    }

    return {intersection_found, shortest_distance};
}

}
