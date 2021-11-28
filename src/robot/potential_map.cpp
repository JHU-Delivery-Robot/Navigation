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

void GradientPotentialMap::updateLidar(std::array<double, SamplesPerRevolution> updated_lidar_scan, double scan_start_heading) {
    lidar_scan = updated_lidar_scan;
    lidar_start_heading = scan_start_heading;
}

common::Vector2 GradientPotentialMap::getAttractivePotential(common::Vector2 position) {
    return attractiveGradientScale * (goal - position);
}

common::Vector2 GradientPotentialMap::getRepulsivePotential() {
    common::Vector2 gradient = common::Vector2(0.0, 0.0);

    for (std::size_t i = 0; i < SamplesPerRevolution; i++) {
        double d = lidar_scan[i];
        if (d <= qStar) {
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
