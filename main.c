#include "gradient_funcs.h"

// no literal map (2D array representing grid), but we're constantly fed
// robot's location and goal points
// (either receive all goal points at once, or one at a time as the robot moves)

int main() {

  // given: start and goal in cartesian, array of lidar data
  // not sure how all these values will be acquired, just variables for now
  Coord x1 = 1;
  Coord y1 = 2;
  Coord x2 = 3;
  Coord y2 = 4;

  // GIVEN - assumed to be in meters
  uint16_t * lidar_data = malloc(sizeof(uint16_t) * N_ANGLES);
  for(int i = 0; i < N_ANGLES; i++) {
    lidar_data[i] = 20;
  }

 

  // turn endpoints in Points
  Vector cur_position = {x1, y1};
  Vector goal = {x2, y2};
  
  // gradient will be a 2D vector!
  Vector attr_gradient = attr_poten_gradient(cur_position, goal);
  Vector rep_gradient = rep_poten_gradient(lidar_data);

  

  free(lidar_data);
  
  return 0;
}
