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

#include "types.hpp"

/**
 * Potential map class
 */
class GradientPotentialMap {
private:
    const int nAngles;       /**< number of ranging measurements per lidar rotation */
    const int qStar;         /**< ??? */
    const int gradientScale; /**< constant used to scale both gradients */
    Vec2d goal;              /**< goal point */
    uint16_t *lidar;         /**< lidar pointer that is in some shared memory */
    /**
     * Given the current position of the robot and the goal, returns the
     * attractive potential gradient vector
     *
     * @param curPosition current position of the robot
     * @return Attractive potential gradient Vector
     */
    Vec2d getAttrPotential(Vec2d position);
    /**
     * Calculates the repulsive potential gradient vector given an array of all
     * obstacle locations in relation to the robot
     *
     * @param lidar array of distances to nearest obstacle at N_ANGLES angles
     * @return Repulsive potential gradient vector
     */
    Vec2d getRepPotential();
public:
    /**
     * Consturct all required args
     */
    GradientPotentialMap(int nAngles, int qStar, int gradientScale, Vec2d goal, uint16_t *lidarData);
    /**
     * Update goal on the potential map.
     *
     * @param goal target of navigation
     */
    void setGoal(Vec2d goal);
    /**
     * Converts the cartesian gradient to radial gradient
     *
     * @param heading current heading of robot
     * @param cartesianGradient the cartesian gradient
     * @return The radial component of the gradient
     */
    double getRadialGradient(Vec2d position, double heading);
    /**
     * Converts the cartesian gradient to radial gradient
     *
     * @param heading current heading of robot
     * @param cartesianGradient the cartesian gradient
     * @return The tangential component of the gradient, which is analogous to
     * the angular velocity
     */
    double getTangentGradient(Vec2d position, double heading);
};
#endif
