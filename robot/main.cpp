#include "poten_map.hpp"
#include "types.hpp"

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
  uint16_t *lidar_data = new uint16_t[Constants::LIDAR_N_ANGLES];
  for(int i = 0; i < Constants::LIDAR_N_ANGLES; i++) {
    lidar_data[i] = 20;
  }

  GradientPotentialMap potentialMap(Constants::LIDAR_N_ANGLES, Constants::Q_STAR,
                                    Constants::GRADIENT_SCALE, {0,0});
  // turn endpoints in Points
  Vec2d cur_position = {x1, y1};
  Vec2d goal = {x2, y2};

  // gradient will be a 2D vector!
  potentialMap.setGoal(goal);
  Vec2d attr_gradient = potentialMap.getAttrPotential(cur_position);
  Vec2d rep_gradient = potentialMap.getRepPotential(lidar_data);



  delete[] lidar_data;

  return 0;
}
