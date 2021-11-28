#include "potential_map.hpp"

#include <cstddef>

#include "util.hpp"
#include "vector2.hpp"

namespace robot {

GradientPotentialMap::GradientPotentialMap(double qStar, double attractiveGradientScale, double repulsiveGradientScale, common::Vector2 goal)
    : qStar(qStar), attractiveGradientScale(attractiveGradientScale), repulsiveGradientScale(repulsiveGradientScale), goal(goal) { }

void GradientPotentialMap::updateGoal(common::Vector2 updatedGoal) {
    goal = updatedGoal;
}

void GradientPotentialMap::updateLidarScan(hal::LidarScanner::Scan updated_lidar_scan) {
    lidar_scan = updated_lidar_scan;
}

common::Vector2 GradientPotentialMap::getAttractivePotential(common::Vector2 position) {
    return attractiveGradientScale * (goal - position);
}

common::Vector2 GradientPotentialMap::getRepulsivePotential() {
    common::Vector2 gradient = common::Vector2(0.0, 0.0);

    for (auto&& point : *lidar_scan) {
        if (point.distance <= qStar) {
            // negate since we want potential to repel from obstacles
            double magnitude = -repulsiveGradientScale*(1.0/d - 1/qStar)/(d*d);
            double angle = lidar_start_heading + 2*PI*i/double(SamplesPerRevolution);

            gradient += common::Vector2::polar(angle, magnitude);
        }
    }

    return gradient;
}

common::Vector2 GradientPotentialMap::getGradient(common::Vector2 position) {
    return getAttractivePotential(position) + getRepulsivePotential();
}

}  // namespace robot
