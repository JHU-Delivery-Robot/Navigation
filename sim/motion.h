/*
 * JHU Deliverbot Navigation Group 
 * Navigation simulation system 
 * Motion models --- header file 
 *
 * @avnoks (GitHub)  5 Jan 2021 (create date)
 *
 */

/* A NOTICE (5 Jan 2020): We're at an early stage.  Feel free to change 
 * things like function names and types to things that make more sense.
 */

/* These likely need to be changed */
#define DELTA_T 1
#define SIGMA_ALPHA 0.1         /* Std dev of noise in first rotation angle */
#define SIGMA_BETA 0.1          /* Std dev of noise in second rotation angle */
#define SIGMA_DELTA 0.1         /* Std dev of noise in distance b/w two rotations */

#ifndef ROBOT
#include "robot.h"
#endif

/* 
 * Update robot's position with noise using odometry motion model
 * Arguments: 
 *   robot 
 *   lin_velo
 *   ang_velo  
 * Returns nothing.
 */
void update_position(Robot robot, double lin_velo, double ang_velo);

