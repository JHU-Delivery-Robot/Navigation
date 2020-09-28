#include "poten_man.h"

int main() {

  // not sure how all these values will be acquired, just variables for now
  Radius goal_r = 1;
  Angle goal_a = 2;
  Radius lidar_data[N_ANGLES];
  for(int i = 0; i < N_ANGLES; i++) {
    lidar_data[i] = 20;
  }

  // create potential map (all initialized to 0)
  Potential_Map * pmap = allocate_default_potential_map(); 

  // calculate and apply attractive potential
  apply_attr_poten(pmap, goal_r, goal_a);

  // calculate and apply repulsive potential
  apply_repulsive_poten(pmap, lidar_data); 

  
  return 0;
}
