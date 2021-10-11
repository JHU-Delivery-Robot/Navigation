#ifndef DIFFERENTIAL_ODOMETRY_HPP
#define DIFFERENTIAL_ODOMETRY_HPP

#include <tuple>

#include "vector2.hpp"

namespace common {

class DifferentialOdometry {
public:
    DifferentialOdometry(double width, double wheel_circumference);

    void setPose(common::Vector2 position, double heading);
    std::tuple<common::Vector2, double> getPose();

    void updateOdometry(double heading, double left_distance, double right_distance);

private:
    double wheel_base_width;
    double wheel_circumference;

    common::Vector2 position;
    double heading;
    double left_wheels_distance, right_wheels_distance;
};

}

#endif
