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
    Vec2d goal;           /**< goal point */
public:
    /**
     * Consturct all required args
     */
    GradientPotentialMap(int nAngles, int qStar, int gradientScale, Vec2d goal);
    /**
     * Update goal on the potential map.
     *
     * @param goal target of navigation
     */
    void setGoal(Vec2d goal);
    /**
     * Given the current position of the robot and the goal, returns the
     * attractive potential gradient vector
     *
     * @param cur_position current position of the robot
     * @return Attractive potential gradient Vector
     */
    Vec2d getAttrPotential(Vec2d cur_position);

    /**
     * Calculates the repulsive potential gradient vector given an array of all
     * obstacle locations in relation to the robot
     *
     * @param lidar array of distances to nearest obstacle at N_ANGLES angles
     * @return Repulsive potential gradient vector
     */
    Vec2d getRepPotential(uint16_t *lidar);
};
#endif
