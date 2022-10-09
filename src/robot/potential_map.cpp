#include "potential_map.hpp"

#include <cstddef>

#include "common/vector2.hpp"

namespace robot {

PotentialMap::PotentialMap(double q_star, double attractive_coefficient, double repulsive_coefficient, double threshold_distance)
    : q_star(q_star),
      attractive_coefficient(attractive_coefficient),
      repulsive_coefficient(repulsive_coefficient),
      threshold_distance(threshold_distance),
      goal(0.0, 0.0) {}

void PotentialMap::updateGoal(common::Vector2 updated_goal) {
    goal = updated_goal;
}

void PotentialMap::updateLidarScan(hal::LidarScanner::Scan updated_lidar_scan) {
    lidar_scan = updated_lidar_scan;
}

common::Vector2 PotentialMap::getAttractivePotential(common::Vector2 position) {
    auto delta = goal - position;
    auto distance = delta.magnitude();

    if (distance < threshold_distance) {
        // Quadratic potential near goal
        return attractive_coefficient * delta;
    } else {
        // Conical/linear potential away from goal
        common::Vector2 direction = delta * (1 / distance);
        return threshold_distance * attractive_coefficient * direction;
    }
}

common::Vector2 PotentialMap::getRepulsivePotential() {
    common::Vector2 gradient = common::Vector2(0.0, 0.0);

    for (auto&& point : *lidar_scan) {
        if (point.distance <= q_star) {
            // negate since we want potential to repel from obstacles
            double d = point.distance;
            double magnitude = -repulsive_coefficient * (1.0 / d - 1 / q_star) / (d * d);
            gradient += common::Vector2::polar(point.angle, magnitude);
        }
    }

    return gradient;
}

common::Vector2 PotentialMap::getGradient(common::Vector2 position) {
    auto attractive = getAttractivePotential(position);
    auto repulsive = getRepulsivePotential();

    return attractive + repulsive;
}

}  // namespace robot
