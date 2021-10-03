#include "potential_map.hpp"

#include "util.hpp"

GradientPotentialMap::GradientPotentialMap(int nAngles, int qStar, int gradientScale,
                                           Vec2d goal, uint16_t *lidarData)
    : nAngles{nAngles},
      qStar{qStar},
      gradientScale{gradientScale},
      goal(goal),
      lidar{lidarData} {}

void GradientPotentialMap::setGoal(Vec2d goal) {
    this->goal = goal;
}

Vec2d GradientPotentialMap::getAttrPotential(Vec2d position) {
    Coord gradientX = (position.x - goal.x) * gradientScale;
    Coord gradientY = (position.y - goal.y) * gradientScale;
    Vec2d gradient = {gradientX, gradientY};
    return gradient;
}

// so we do need to convert the lidar point to cartesian!
Vec2d GradientPotentialMap::getRepPotential() {
    Coord gradientX = 0;
    Coord gradientY = 0;
    for (int i = 0; i < nAngles; i++) {
        if (lidar[i] <= qStar) {
            double coef = gradientScale * (1.0 / qStar - 1.0 / lidar[i]) * (1.0 / (lidar[i] * lidar[i] * lidar[i]));
            double theta = degreesToRadians((float)i / ((float)nAngles / 360.0));
            // finding dist between current pt (q) and obstacle point (q_goal)
            double x = (-1) * lidar[i] * cos(theta);  // multiplied by -1 because it's robot - obsatcle pt
            double y = (-1) * lidar[i] * sin(theta);  // and trig gives obstacle - robot (triangle)
            gradientX += x * coef;
            gradientY += y * coef;
        }
    }
    Vec2d gradient = {(Coord)gradientX, (Coord)gradientY};
    return gradient;
}

double GradientPotentialMap::getRadialGradient(Vec2d position, double heading) {
    Vec2d cartesianGradient = getAttrPotential(position) + getRepPotential();
    return cos(heading) * cartesianGradient.x + sin(heading) * cartesianGradient.y;
}

double GradientPotentialMap::getTangentGradient(Vec2d position, double heading) {
    Vec2d cartesianGradient = getAttrPotential(position) + getRepPotential();
    double tanAng = heading + PI / 2;
    return cos(tanAng) * cartesianGradient.x + sin(tanAng) * cartesianGradient.y;
}
