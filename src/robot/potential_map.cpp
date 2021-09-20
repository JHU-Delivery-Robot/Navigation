#include "potential_map.hpp"

#include "util.hpp"
#include "vector2.hpp"

namespace robot {

GradientPotentialMap::GradientPotentialMap(int qStar, int gradientScale,
                                           common::Vector2 goal, std::array<double, SamplesPerRevolution> lidar_scan)
    :  qStar(qStar), gradientScale(gradientScale), goal(goal), lidar_scan(lidar_scan) { }

void GradientPotentialMap::updateGoal(common::Vector2 updatedGoal) {
    goal = updatedGoal;
}

void GradientPotentialMap::updateLidar(std::array<double, SamplesPerRevolution> updated_lidar_scan) {
    lidar_scan = updated_lidar_scan;
}

common::Vector2 GradientPotentialMap::getAttractivePotential(common::Vector2 position) {
    return gradientScale * (goal - position);
}

common::Vector2 GradientPotentialMap::getRepulsivePotential() {
    common::Vector2 gradient = common::Vector2(0.0, 0.0);

    for (size_t i = 0; i < SamplesPerRevolution; i++)
    {
        double d = lidar_scan[i];
        if (d <= qStar) {
            // negate since we want potential to repel from obstacles
            double magnitude = -gradientScale*(1.0/qStar - 1.0/d)/(d*d*d);
            double angle = 2*PI*i/double(SamplesPerRevolution);

            gradient += common::Vector2::polar(angle, magnitude);
        }
    }

    return gradient;
}

common::Vector2 GradientPotentialMap::getGradient(common::Vector2 position) {
    return getAttractivePotential(position) + getRepulsivePotential();
}

}
