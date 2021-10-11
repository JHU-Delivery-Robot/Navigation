/**
 * JHU Deliverbot Navigation Group
 * Navigation simulation system
 * Potential maps - header file
 *
 * @clevitt16 (Github)
 */
#ifndef POTENIAL_MAP_HPP
#define POTENIAL_MAP_HPP

#include <array>

#include "vector2.hpp"
//#include "lidar_scanner.hpp"

constexpr int SamplesPerRevolution = 720;

namespace robot {

class GradientPotentialMap {
public:
    GradientPotentialMap(double qStar, double attractiveGradientScale, double repulsiveGradientScale, common::Vector2 goal);

    void updateGoal(common::Vector2 goal);
    void updateLidar(std::array<double, SamplesPerRevolution> lidar_scan, double scan_start_heading);

    common::Vector2 getAttractivePotential(common::Vector2 position);
    common::Vector2 getRepulsivePotential();

    /**
     * Calculates the gradient of the navigation potential at the specified position
     */
    common::Vector2 getGradient(common::Vector2 position);

private:
    double qStar;
    double attractiveGradientScale;
    double repulsiveGradientScale;

    common::Vector2 goal;

    double lidar_start_heading;
    std::array<double, SamplesPerRevolution> lidar_scan;
};

}

#endif
