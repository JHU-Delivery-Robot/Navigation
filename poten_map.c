/*                                    
 * JHU Deliverbot Navigation Group     
 * Navigation simulation system  
 * Potential maps --- implementation file   
 *                                                                                                      
 * @avnoks (GitHub)  23 Sep 2020
 * 
 */

/* A NOTICE (23 Sep 2020): We're at an early stage.  Feel free to change 
 * things like function names and types to things that make more sense.
 */

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "poten_map.h"
#include <stdlib.h>

#define PI 3.14159265358979323

/* ----------------------------------------------------------------------      
 *   Memory managment                                                    
 * ---------------------------------------------------------------------- */

Potential_Map *allocate_default_potential_map() {
  Potential_Map * pmap = malloc(sizeof(Potential_Map));
  pmap->map = calloc(N_ANGLES * N_DISTANCES, sizeof(Potential));
  pmap->n_angles = N_ANGLES;
  pmap->n_distances = N_DISTANCES;
  return pmap;
}

void destroy_potential_map(Potential_Map *pmap) {
  free(pmap->map);
  free(pmap);
}


/* ----------------------------------------------------------------------                               
 *   Potential functions                                                                                
 * ---------------------------------------------------------------------- */

// Converts degrees to radians.  
#define degreesToRadians(angleDegrees) (angleDegrees * PI / 180.0)

// Converts radians to degrees.        
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / PI)

double euclidean_dist_squared(Radius r1, Angle theta1, Radius r2, Angle theta2) {
  double radiantheta1 = degreesToRadians(theta1);
  double radiantheta2 = degreesToRadians(theta2);
  double distance_square = pow((double)r1, 2.0)+pow((double)r2, 2.0) - 2*r1*r2*cos(radiantheta2 - radiantheta1);
  return distance_square;
}



Potential attractive_potential(Radius r, Angle theta, Radius goal_r, Angle goal_theta) {
  double ARBITRARY_CONSTANT = 0.1;
  double squareDistance = euclidean_dist_squared(r, theta, goal_r, goal_theta);
  return (Potential)0.5*ARBITRARY_CONSTANT*squareDistance;
}



Potential repulsive_potential(Radius r, Radius obstacle_r) {
  Radius dist = obstacle_r - r;
  if (dist > Q_STAR_REPULSIVE) {
    return (Potential)0;
  }
  else if (dist <= 0) { // behind the obstacle, apply max potential (dist is 0)
    return (Potential)(0.5)*(0.1)*(pow((double)(1.0/1)-(1.0/Q_STAR_REPULSIVE), (double)2));
  }
  else {
    double temp = pow((double)(1.0/dist)-(1.0/Q_STAR_REPULSIVE), (double)2);
    return (Potential)(0.5)*(0.1)*(temp); // where 0.1 is the arbitrary constant                         
  }
}


Potential trench_potential(Radius d) {
  double temp = pow((double)d, (double)2);
  return (Potential)(0.5)*(0.1)*temp; // where 0.1 is the constant k                                          
}


void apply_attr_poten(Potential_Map *pmap, Radius goal_r, Angle goal_theta) {
  Potential * cur  = pmap->map;
    for(Angle a = 0; a < pmap->n_angles; a++) {
    for(Radius r = 0; r < pmap->n_distances; r++) {
      *cur += attractive_potential(r, a, goal_r, goal_theta);
      cur++;
    }
  }
}

void apply_repulsive_poten(Potential_Map *pmap, Radius* lidar_data) {
  Potential * cur = pmap->map;
    for(Angle a = 0; a < pmap->n_angles; a++) {
    for(Radius r = 0; r < pmap->n_distances; r++) {
      *cur += repulsive_potential(r, lidar_data[a]);
      cur++;
    }
  }
}


double find_slope(Radius x1, Radius y1, Radius x2, Radius y2) {
  return (y2-y1)/(double)(x2-x1);
}


Angle inverse_tangent(Radius x_val, Radius y_val, uint16_t num_angles) {
  if (x_val == 0) {
    if (y_val >= 0) {
      return num_angles / 4 - 1; // pi/2
    }
    return num_angles * 3 / 4 - 1; // 3pi/2
  }
  double theta_rads = atan((double)y_val/x_val);
  if (x_val < 0) {
    theta_rads += PI;
  }
  Angle theta_deg = radiansToDegrees(theta_rads);
  return (Angle)theta_deg*((double)num_angles/360);
}


Angle radial_line(Radius x1, Radius y1, Radius x2, Radius y2, uint16_t num_angles) {
  double slope = find_slope(x1, y1, x2, y2);
  return inverse_tangent(slope, -1, num_angles);
}


Polar_Path create_polar_path(Radius x1, Radius y1, Radius x2, Radius y2, uint16_t num_angles) {
  Polar_Path pol_path;
  pol_path.theta_start = inverse_tangent(x1, y1, num_angles);
  pol_path.theta_end = inverse_tangent(x2, y2, num_angles);
  if (x1 == x2) { // vertical line
    if (x1 >= 0) {
      pol_path.gamma = 0;
    }
    else {
      pol_path.gamma = num_angles / 2 - 1;  // pi
    }
    pol_path.r0 = abs(x1);
    return pol_path;
  }
  else if (y1 == y2) { // horizontal line
    if (y1 >= 0) {
      pol_path.gamma = num_angles / 4 - 1;  // pi/2
    }
    else {
      pol_path.gamma = num_angles * 3 / 4 - 1; // 3pi/2
    }
    pol_path.r0 = abs(y1);
    return pol_path;
  }
  pol_path.gamma = radial_line(x1, y1, x2, y2, num_angles);
  pol_path.r0 = find_intersection(x1, y1, x2, y2);
  return pol_path;
}


void apply_trench_poten(Polar_Path pol_path, Potential_Map *pmap) {
  for(int i = 0; i < Q_STAR_TRENCH; i++) {
    Potential poten = trench_potential(i);
    for(Angle a = pol_path.theta_start; a <= pol_path.theta_end; a++) {
      // calculate radius at each angle, then use radius-angle
      // pair to find correct spot in potential map, and add potential
    }
  }
}
