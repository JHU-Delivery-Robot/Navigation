#ifndef POTEN_MAP_HPP
#define POTEN_MAP_HPP

/*
 *
 * JHU Deliverbot Navigation Group
 * Navigation simulation system
 * Potential maps - header file
 *
 * @clevitt16 (Github) 
 */

/* ----------------------------
 *    Defines, typedefs
 * -------------------------------- */

#include <cstdint>
#include <cstdlib>

#define N_ANGLES 720  // number of measurements returned by the lidar scan 
#define Q_STAR 10
#define PI 3.14159265358979323

typedef int32_t Coord; // signed or unsigned?

typedef struct {
  Coord x;
  Coord y;
} Vector;


/* --------------------------
 *    Function declarations
 * ----------------------------- */

/*
 * Given the current position of the robot and the goal,
 * returns the attractive potential gradient vector
 * Parameters:
 *   cur_position - current position of the robot
 *   goal         - goal position of the robot
 * Returns
 *   Attractive potential gradient Vector
 *
 */
Vector attr_poten_gradient(Vector cur_position, Vector goal);

/*
 * Calculates the repulsive potential gradient vector
 * given an array of all obstacle locations in relation
 * to the robot
 * Patameters:
 *   lidar - array of distances to nearest obstacle at N_ANGLES angles
 * Returns:
 *   Repulsive potential gradient vector
 */
Vector rep_poten_gradient(uint16_t * lidar);

#endif