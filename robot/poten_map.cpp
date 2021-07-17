#include "poten_map.hpp"
#include <math.h>

#define degreesToRadians(angleDegrees) (angleDegrees * PI / 180.0)

Vector attr_poten_gradient(Vector cur_position, Vector goal) {
  #define CONSTANT 1
  Coord gradient_x = (cur_position.x - goal.x) * CONSTANT;
  Coord gradient_y = (cur_position.y - goal.y) * CONSTANT;
  Vector gradient = {gradient_x, gradient_y};
  return gradient;
}

// so we do need to convert the lidar point to cartesian! 
Vector rep_poten_gradient(uint16_t * lidar) {
  #define CONSTANT 1
  Coord gradient_x = 0;
  Coord gradient_y = 0;
  for(int i = 0; i < N_ANGLES; i++) {
    if (lidar[i] <= Q_STAR) {
      double coef = CONSTANT*(1.0/Q_STAR - 1.0/lidar[i])*(1.0/(lidar[i]*lidar[i]*lidar[i]));
      double theta = degreesToRadians(i/(N_ANGLES/360));
      // finding dist between current pt (q) and obstacle point (q_goal)
      double x = (-1)*lidar[i]*cos(theta); // multiplied by -1 because it's robot - obsatcle pt
      double y = (-1)*lidar[i]*sin(theta); // and trig gives obstacle - robot (triangle)
      gradient_x += x*coef;
      gradient_y += y*coef;
    }
  }
  Vector gradient = {(Coord)gradient_x, (Coord)gradient_y};
  return gradient;
}

////////////////////
// Functions that convert cartesian to polar gradients

// Arguments
//   heading - current heading (angular position) of robot
//   cartesian_gradient - [x,y] gradient calculated from potential map
// Returns linear (forward/backward) velocity
double cartesian_to_radial_gradient (double heading, Vector cartesian_gradient) {
  return cos(heading) * cartesian_gradient.x + sin(heading) * cartesian_gradient.y;
}

// Arguments
//   heading - current heading (angular position) of robot
//   cartesian_gradient - [x,y] gradient calculated from potential map
// Returns angular velocity
double cartesian_to_tangent_gradient (double heading, Vector cartesian_gradient) {
  double tan_ang  = heading + PI/2;
  return cos(tan_ang) * cartesian_gradient.x + sin(tan_ang) * cartesian_gradient.y;
}
