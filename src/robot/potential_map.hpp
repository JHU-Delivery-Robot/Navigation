/**
 * JHU Deliverbot Navigation Group
 * Navigation simulation system
 * Potential maps - header file
 *
 * @clevitt16 (Github)
 */
#ifndef POTEN_MAP_HPP
#define POTEN_MAP_HPP

#include <cstdint>

#include "vector2.hpp"

/**
 * Potential map class
 */
class GradientPotentialMap
{
private:
    const int nAngles;       /**< number of ranging measurements per lidar rotation */
    const int qStar;         /**< ??? */
    const int gradientScale; /**< constant used to scale both gradients */
    common::Vector2 goal;
    uint16_t *lidar; /**< lidar pointer that is in some shared memory */
    /**
     * Given the current position of the robot and the goal, returns the
     * attractive potential gradient vector
     *
     * @param curPosition current position of the robot
     * @return Attractive potential gradient Vector
     */
    common::Vector2 getAttrPotential(common::Vector2 position);
    /**
     * Calculates the repulsive potential gradient vector given an array of all
     * obstacle locations in relation to the robot
     *
     * @param lidar array of distances to nearest obstacle at N_ANGLES angles
     * @return Repulsive potential gradient vector
     */
    common::Vector2 getRepPotential();

public:
    /**
     * Consturct all required args
     */
    GradientPotentialMap(int nAngles, int qStar, int gradientScale, common::Vector2 goal, uint16_t *lidarData);
    /**
     * Update goal on the potential map.
     *
     * @param goal target of navigation
     */
    void setGoal(common::Vector2 goal);
    /**
     * Converts the cartesian gradient to radial gradient
     *
     * @param heading current heading of robot
     * @param cartesianGradient the cartesian gradient
     * @return The radial component of the gradient
     */
    double getRadialGradient(common::Vector2 position, double heading);
    /**
     * Converts the cartesian gradient to radial gradient
     *
     * @param heading current heading of robot
     * @param cartesianGradient the cartesian gradient
     * @return The tangential component of the gradient, which is analogous to
     * the angular velocity
     */
    double getTangentGradient(common::Vector2 position, double heading);
};
#endif
