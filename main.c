#include "poten_map.h"
#include <math.h>
#include <stdlib.h>

int main() {

  // given: start and goal in cartesian, array of lidar data
  // not sure how all these values will be acquired, just variables for now
  Cart_dist x1 = 1;
  Cart_dist y1 = 2;
  Cart_dist x2 = 3;
  Cart_dist y2 = 4;
  
  Radius lidar_data[N_ANGLES];
  for(int i = 0; i < N_ANGLES; i++) {
    lidar_data[i] = 20;
  }

  // create potential map (all initialized to 0)
    Potential_Map * pmap = allocate_default_potential_map(); 

  
  // turn endpoints in Points
  Point start = {x1, y1, sqrt(x1*x1 + y1*y1), arctan(x1, y1, pmap->n_angles)}; 
  Point goal = {x2, y2, sqrt(x2*x2 + y2*y2), arctan(x2, y2, pmap->n_angles)};
  

  // calculate and apply attractive potential
  apply_attr_poten(pmap, goal.radius, goal.angle);

  // calculate and apply repulsive potential
  apply_repulsive_poten(pmap, lidar_data); 

  Polar_Path * pol_path = create_polar_path(start, goal, pmap->n_angles);

  apply_trench_poten(pol_path, pmap, pmap->n_angles);

  free(pol_path);
  destroy_potential_map(pmap);
  
  return 0;
}
