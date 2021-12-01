#ifndef POTENTIAL_MAP_HPP
#define POTENTIAL_MAP_HPP

#include <array>

#include "lidar_scanner.hpp"
#include "vector2.hpp"

namespace robot {

/**
 * PotentialMap provides a navigational potential function. By constructing a function
 * with high potential near obstacles, and decreasing potential near a target location,
 * a robot can navigate by greedily following the negative of the gradient. Note that
 * obstacles may create local minima which the robot could get stuck in. Higher-level
 * route planning is needed to avoid this.
 * 
 * PotentialMap accounts for obstacles when calculating the gradient using a lidar scan
 * of the robot's current surroundings. The lidar scan should be updated whenever a new
 * scan is available.
 */
class PotentialMap {
public:
    PotentialMap(double qStar, double attractive_coefficient, double repulsive_coefficient, double threshold_distance);

    void updateGoal(common::Vector2 goal);
    void updateLidarScan(hal::LidarScanner::Scan updated_lidar_scan);

    common::Vector2 getAttractivePotential(common::Vector2 position);
    common::Vector2 getRepulsivePotential();

    /**
     * Calculates the gradient of the navigation potential at the specified position
     */
    common::Vector2 getGradient(common::Vector2 position);

private:
    double q_star;
    double attractive_coefficient;
    double repulsive_coefficient;
    double threshold_distance;

    common::Vector2 goal;

    hal::LidarScanner::Scan lidar_scan;
};

}  // namespace robot

#endif
