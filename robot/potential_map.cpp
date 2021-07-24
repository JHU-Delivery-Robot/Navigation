#include "potential_map.hpp"

#include "util.hpp"

GradientPotentialMap::GradientPotentialMap(int nAngles, int qStar, int gradientScale,
                                           Vec2d goal) :
    nAngles{nAngles},
    qStar{qStar},
    gradientScale{gradientScale},
    goal(goal) {}

void GradientPotentialMap::setGoal(Vec2d goal) {
    this->goal = goal;
}

Vec2d GradientPotentialMap::getAttrPotential(Vec2d cur_position) {
  Coord gradient_x = (cur_position.x - goal.x) * gradientScale;
  Coord gradient_y = (cur_position.y - goal.y) * gradientScale;
  Vec2d gradient = {gradient_x, gradient_y};
  return gradient;
}

// so we do need to convert the lidar point to cartesian!
Vec2d GradientPotentialMap::getRepPotential(uint16_t *lidar) {
  Coord gradient_x = 0;
  Coord gradient_y = 0;
  for(int i = 0; i < nAngles; i++) {
    if (lidar[i] <= qStar) {
      double coef = gradientScale*(1.0/qStar - 1.0/lidar[i])*(1.0/(lidar[i]*lidar[i]*lidar[i]));
      double theta = degreesToRadians((float) i/((float) nAngles/360.0));
      // finding dist between current pt (q) and obstacle point (q_goal)
      double x = (-1)*lidar[i]*cos(theta); // multiplied by -1 because it's robot - obsatcle pt
      double y = (-1)*lidar[i]*sin(theta); // and trig gives obstacle - robot (triangle)
      gradient_x += x*coef;
      gradient_y += y*coef;
    }
  }
  Vec2d gradient = {(Coord)gradient_x, (Coord)gradient_y};
  return gradient;
}
