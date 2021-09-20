#include "potential_map.hpp"

#include <cmath>

#include "types.hpp"
#include "vector2.hpp"
#include "util.hpp"

GradientPotentialMap::GradientPotentialMap(int nAngles, int qStar, int gradientScale,
                                           common::Vector2 goal, uint16_t *lidarData)
    : nAngles(nAngles),
      qStar{qStar},
      gradientScale{gradientScale},
      goal(goal),
      lidar{lidarData} {}

void GradientPotentialMap::setGoal(common::Vector2 goal)
{
  this->goal = goal;
}

common::Vector2 GradientPotentialMap::getAttrPotential(common::Vector2 position)
{
  Coord gradientX = (position.x - goal.x) * gradientScale;
  Coord gradientY = (position.y - goal.y) * gradientScale;
  common::Vector2 gradient = common::Vector2(gradientX, gradientY);
  return gradient;
}

// so we do need to convert the lidar point to cartesian!
common::Vector2 GradientPotentialMap::getRepPotential()
{
  Coord gradientX = 0;
  Coord gradientY = 0;
  for (int i = 0; i < nAngles; i++)
  {
    if (lidar[i] <= qStar)
    {
      double coef = gradientScale * (1.0 / qStar - 1.0 / lidar[i]) * (1.0 / (lidar[i] * lidar[i] * lidar[i]));
      double theta = 2 * PI * ((float)i / (float)nAngles);
      // finding dist between current pt (q) and obstacle point (q_goal)
      double x = (-1) * lidar[i] * cos(theta); // multiplied by -1 because it's robot - obsatcle pt
      double y = (-1) * lidar[i] * sin(theta); // and trig gives obstacle - robot (triangle)
      gradientX += x * coef;
      gradientY += y * coef;
    }
  }

  common::Vector2 gradient = common::Vector2((Coord)gradientX, (Coord)gradientY);
  return gradient;
}

double GradientPotentialMap::getRadialGradient(common::Vector2 position, double heading)
{
  common::Vector2 cartesianGradient = getAttrPotential(position) + getRepPotential();
  return cos(heading) * cartesianGradient.x + sin(heading) * cartesianGradient.y;
}

double GradientPotentialMap::getTangentGradient(common::Vector2 position, double heading)
{
  common::Vector2 cartesianGradient = getAttrPotential(position) + getRepPotential();
  double tanAng = heading + PI / 2;
  return cos(tanAng) * cartesianGradient.x + sin(tanAng) * cartesianGradient.y;
}
