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
  //link to the source: https://socratic.org/questions/what-is-the-formula-for-the-distance-between-two-polar-coordinates
  double radiantheta1 = degreesToRadians(theta1);
  double radiantheta2 = degreesToRadians(theta2);
  double distance_square = r1 * r1 + r2 * r2 - 2*r1*r2*cos(radiantheta2 - radiantheta1);
  return distance_square;
}



Potential attractive_potential(Radius r, Angle theta, Radius goal_r, Angle goal_theta) {
  #define ARBITRARY_CONSTANT 0.1
  double squareDistance = euclidean_dist_squared(r, theta, goal_r, goal_theta);
  return (Potential)0.5*ARBITRARY_CONSTANT*squareDistance;
}



Potential repulsive_potential(Radius r, Radius obstacle_r) {
  #define CONSTANT 0.1
  Radius dist = obstacle_r - r;
  if (dist > Q_STAR_REPULSIVE) {
    return (Potential)0;
  }
  else if (dist <= 0) { // behind the obstacle, apply max potential (dist is 0)
    double temp = ((1.0/dist)-(1.0/Q_STAR_REPULSIVE))*((1.0/dist)-(1.0/Q_STAR_REPULSIVE));
    return (Potential)(0.5)*(CONSTANT)*(temp);
  }
  else {
    double temp = ((1.0/dist)-(1.0/Q_STAR_REPULSIVE))*((1.0/dist)-(1.0/Q_STAR_REPULSIVE));
    return (Potential)(0.5)*CONSTANT*(temp); 
  }
}


Potential trench_potential(Radius d) {
  #define SOME_CONSTANT 0.1
  double temp = d * d;
  return (Potential)(0.5)*SOME_CONSTANT*temp; 
}


void edit_potential_map(Potential_Map *pmap, Angle a, Radius r, Potential poten) {
  while (a < 0) {
    a += pmap->n_angles;
  }
  while (a >= pmap->n_angles) {
    a -= pmap->n_angles;
  }
  (pmap->map)[a * pmap->n_distances + r] += poten;
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


double find_slope(Cart_dist x1, Cart_dist y1, Cart_dist x2, Cart_dist y2) {
  return (y2-y1)/(double)(x2-x1);
}


Angle arctan(Cart_dist x_val, Cart_dist y_val, uint16_t num_angles) {
  if (x_val == 0) {
    if (y_val >= 0) {
      return num_angles / 4 - 1; // pi/2
    }
    return num_angles * 3 / 4 - 1; // 3pi/2
  }
  double theta_rads = atan2((double)y_val, (double)x_val); // should return angle in correct quadrant
  if (theta_rads < 0) { // correcting range
    theta_rads += 2 * PI;
  }
  Angle theta_deg = radiansToDegrees(theta_rads);
  return (Angle)theta_deg*((double)num_angles/360);
}


double radial_line(Cart_dist x1, Cart_dist y1, Cart_dist x2, Cart_dist y2) {
  double slope = find_slope(x1, y1, x2, y2);
  double gamma = atan(-1/slope); // range: -PI/2,PI/2
  // gamma should be less than (or equal to) 90deg from either endpoint angle
  double endpoint_angle;
  if (x1 == 0) {
    endpoint_angle = atan2((double)y2, (double)x2); // more efficient to pass in Point struct ?
  } // have precondition that x1 != x2, so we know x1 != 0
  else {
    endpoint_angle = atan2((double)y1, (double)x1);
  }
  if (fabs(gamma - endpoint_angle) > PI/2) { // gamma: -PI/2,PI/2   endpt: -PI,PI 
    gamma += PI;
  }
  if (gamma <= 0) {
    gamma += 2 * PI;;
  }
  return gamma;
}


Polar_Path * create_polar_path(Point start_point, Point goal_point, uint16_t num_angles) {
  Polar_Path * path = malloc(sizeof(Polar_Path));
  path->start = start_point;
  path->goal = goal_point;
  if (start_point.x == goal_point.x) { // vertical line!
    if (start_point.x >= 0) {
      path->gamma = 0;
      path->r0.angle = 0;
    }
    else {
      path->gamma = PI;
      path->r0.angle = (Angle)(num_angles / 2 - 1);
    }
    path->r0.radius = (Radius)abs(start_point.x);
  }
  else if (start_point.y == goal_point.y) { // horizontal line
    if (start_point.y >= 0) {
      path->gamma = PI / 2;
      path->r0.angle = (Angle)(num_angles / 4 - 1);
    }
    else {
      path->gamma = 3 * PI / 2; // 3pi/2
      path->r0.angle = (Angle)(3 * num_angles / 4 - 1);
    }
    path->r0.radius = (Radius)abs(start_point.y);
  }
  else {
    path->gamma = radial_line(start_point.x, start_point.y, goal_point.x, goal_point.y); 
    path->r0.angle = (Angle)(radiansToDegrees(path->gamma) * (num_angles / 360.0));
    path->r0.radius = find_intersection(start_point.x, start_point.y, goal_point.x, goal_point.y);
  }
  path->r0.x = (Cart_dist)(path->r0.radius * cos(path->gamma));
  path->r0.y = (Cart_dist)(path->r0.radius * sin(path->gamma));
  return path;
}


Radius find_intersection(Cart_dist x1, Cart_dist y1, Cart_dist x2, Cart_dist y2) {
  Radius intersection;
  if((x1 == 0 && y1 == 0) || (x2 == 0 && y2 == 0)){
    return 0;
  }
  else {
    double m = find_slope(x1, y1, x2, y2);
    Radius xradial = (m*x1-y1)/(m+1/m);
    Radius yradial = -xradial/m;
    intersection = sqrt(xradial * xradial + yradial * yradial);
    return intersection;
  }
}
    



void apply_trench_poten(Polar_Path * path, Potential_Map *pmap, uint16_t num_angles) {
  // find distances from r0 to endpoints (constant for whole trench)
  Cart_dist x_diff = path->start.x - path->r0.x;
  Cart_dist y_diff = path->start.y - path->r0.y;
  double r0_to_start = sqrt((double)x_diff * x_diff + y_diff * y_diff);
  x_diff = path->goal.x - path->r0.x;
  y_diff = path->goal.y - path->r0.y;
  double r0_to_end = sqrt((double)x_diff * x_diff + y_diff * y_diff);
  // begin iterating through parallel lines
  for (uint8_t i = -Q_STAR_TRENCH; i < Q_STAR_TRENCH; i++) {
    Potential poten = trench_potential(i);
    Radius cur_r0;
    double cur_gamma = path->gamma;
    if (i > path->r0.radius) {    // flipping gamma if this parallel's r0 crosses origin ?
      cur_r0 = i - path->r0.radius;
      if (cur_gamma >= PI) {
	cur_gamma -= PI;
      }
      cur_gamma += PI;
    }
    else {
      cur_r0 = path->r0.radius - i;
    }
    
    if (cur_r0 == 0) { // radial line!!
      // Case 1: original path, does not cross origin
      if (path->start.angle == path->goal.angle) {
	Radius small = (path->start.radius <= path->goal.radius) ? path->start.radius : path->goal.radius;
	Radius large = (path->start.radius <= path->goal.radius) ? path->goal.radius : path->start.radius;
	for(Radius r = small; r < large; r++) {
	  edit_potential_map(pmap, path->start.angle, r, poten);
	}
      }
    
      // Case 2: original path, crosses origin
      else if (abs(path->start.angle - path->goal.angle) == num_angles/2) {
	for(Radius r = 0; r < path->start.radius; r++) {
	  edit_potential_map(pmap, path->start.angle, r, poten);
	}
	for(Radius r = 1; r < path->goal.radius; r++) {
	  edit_potential_map(pmap, path->goal.angle, r, poten);
	}
      }
      
      // Case 3: radial line is a parallel path!
      else if (i != 0) {
	// cur_r0 is the origin!!
	// can visualize that path makes a transformation to the origin
	Cart_dist new_start_x = path->start.x - path->r0.x;
	Cart_dist new_start_y = path->start.y - path->r0.y;
	Cart_dist new_goal_x = path->goal.x - path->r0.x;
	Cart_dist new_goal_y = path->goal.y - path->r0.y;
	// good to do a whole new arctan, instead of perpendicular to gamma?
	Angle angle_start = arctan(new_start_x, new_start_y, num_angles);
	Angle angle_end = arctan(new_goal_x, new_goal_y, num_angles);
	// is this a better way to calculate dist btw r0 and an endpoint? no, not guaranteed to happen
	// Case 3.1 radial parallel does not cross origin
	if (angle_start == angle_end) {
	  Radius small = (r0_to_start <= r0_to_end) ? (Radius)r0_to_start : (Radius)r0_to_end;
	  Radius large = (r0_to_start <= r0_to_end) ? (Radius)r0_to_end : (Radius)r0_to_start;
	  for(Radius r = small; r <= large; r++) {
	    edit_potential_map(pmap, angle_start, r, poten);
	  }
	}
	// Case 3.2 radial path does cross origin
	else {
	  for(Radius r = 0; r < (Radius)r0_to_start; r++) {
	    edit_potential_map(pmap, angle_start, r, poten);
	  }
	  for(Radius r = 1; r < (Radius)r0_to_end; r++) {
	    edit_potential_map(pmap, angle_end, r, poten);
	  }
	}
      }
      return;
    }
  
    // assuming we need to calculate a new angle range (consider initial path ?)
    // also assuming that path crosses cur_r0 (no negative distances yet)
    Angle cur_theta_start;
    Angle cur_theta_end;
    if (i == 0) {
      cur_theta_start = path->start.angle;
      cur_theta_end = path->goal.angle;
    }
    else {
      double gamma_start_ang = atan(r0_to_start/(double)cur_r0); // r0 != 0 (radial^^), -PI/2,PI/2
      double gamma_end_ang = atan(r0_to_end/(double)cur_r0); // both inputs should be + so arctan is +
      // now must figure out whether to add or subtract these angles from gamma!
      // we know start and end are within 90deg of gamma - will check which side!
      if (path->start.angle > path->r0.angle || path->r0.angle - path->start.angle > 3*num_angles/4) {
	cur_theta_start = path->r0.angle + radiansToDegrees(gamma_start_ang)*(num_angles/360.0);
      }
      else {
	cur_theta_start = path->r0.angle - radiansToDegrees(gamma_start_ang);
      }
      if (path->goal.angle > path->r0.angle || path->r0.angle - path->goal.angle > 3*num_angles/4) {
	cur_theta_end = path->r0.angle + radiansToDegrees(gamma_end_ang)*(num_angles/360.0);
      }
      else {
	cur_theta_end = path->r0.angle - radiansToDegrees(gamma_end_ang)*(num_angles/360.0);
      }
    }
    // angles not assumed to be in-range, wrapper function handles that
    // note: angle type is not unsigned - can be negative, potential map wrapper handles it
    Angle small = (cur_theta_start <= cur_theta_end) ? cur_theta_start : cur_theta_end;
    Angle large = (cur_theta_start <= cur_theta_end) ? cur_theta_end : cur_theta_start;
    for(Angle a = small; a < large; a++) {
      double a_rads = (double)degreesToRadians(a*(360.0/num_angles));
      Radius r = (Radius)(cur_r0 * (1/cos(a_rads - path->gamma)));
      edit_potential_map(pmap, a, r, poten);
    }

  }
}

  // there are 4 special cases
  // 1: path is radial but does not cross origin
  //    r0 will still be 0 - can check that thetas are same
  //    then can start at rstart and incriment/decriment to rend
  // 2: path is radial and crosses origin
  //    r0 will be 0 - must check that thetas are opposites
  //    then can start at each r on respective thetas and go to 0
  // 3: path is nonradial, but a parallel is radial, does not cross origin
  //    r0 will become 0 - check for i != 0 and something for no origin cross
  //    need to find the angle of this radial line, rstart, rend
  //      angle will be perpendicular to gamma ? but on which side?
  //      
  // 4: path is nonraidal, parallel is radial, crosses origin
  //    r0 will become 0 - check for i != 0 and something for origin cross
  //    radial angle will be perpendicular to gamma, but need rs
  //      angle will be perpendicular to gamma
  //      radii will be the distance from r0 to each original endpoint

// Info we have coming into applying tranch potential:
//   polar and cartesian coordinates of path endpoints
//   gamma, which is angle purpendicular
//   polar coordinates of intersection point (cartesian? or can find using rsin0, rcos0)

// Brainstorming:
// We will need to calculate a new angle range for each parallel line!
// I think this means that the two sides of the trench can't be done
//   simultaneously anymore (since each line has a different angle range)
// How will we do this?
//   Find angles between parallel line radii and gamma
//     Will use inverse tangent
//     Means we need tangent! will use ~r0 and distance between ~r0 and parallel endpoint
//     dist between ~r0 and || endpoint is same as dist from r0 to orig endpoint
//     note: if parallel segment is on one side of gamma line, dist btw ~r0 and || endp
//           will have to be negative to create the triangle


// How to deal with the radial parallel lines
//  Can still check for ~r == 0, then not recalculate angles
//  then follow steps above ?

// Final trench potential algorithm
// 1) Begin at one edge of Q* (r0 - Q*), then add until r0 + Q*
//    note: if r0 - Q* (~r0) becomes negative, need to flip everything?
// 2) Calculate trench potential at that ~r0
// 3) If ~r0 == 0, it's a radial line!! Find correct angle and radii to traverse
//    to fill in potential for that line (described above)
// 4) Find new angle range (described above)
// 5) iterate through angle range, calculate radii on line, update potential map!
