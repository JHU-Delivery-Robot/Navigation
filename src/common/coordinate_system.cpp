#include "coordinate_system.hpp"

#include <cmath>

#include "common.hpp"

namespace common {

CoordinateSystem::CoordinateSystem(Coordinates origin) : origin(origin) {}

Vector2 CoordinateSystem::project(Coordinates coordinates) {
    double theta = radians(coordinates.longitude - origin.longitude);
    double phi = radians(coordinates.latitude);

    double secant_theta = 1.0 / std::cos(theta);
    double y = std::atan2(secant_theta * std::sin(phi), secant_theta * std::cos(phi));
    double x = std::atanh(std::sin(theta) * std::cos(phi));

    return Vector2(earth_mean_radius_m * x, earth_mean_radius_m * y);
}

Coordinates CoordinateSystem::project(Vector2 position) {
    double x = position.x / earth_mean_radius_m;
    double y = position.y / earth_mean_radius_m;

    double theta = std::atan(std::sinh(x) / std::cos(y));
    double phi = std::asin(std::sin(y) / std::cosh(x));

    return Coordinates(degrees(phi), degrees(theta) + origin.longitude);
}

std::ostream& operator<<(std::ostream& output, const CoordinateSystem& coordinate_system) {
    return output << "spherical transverse Mercator projection with meridian at " << coordinate_system.origin.longitude << " degrees";
}

constexpr double CoordinateSystem::radians(double degrees) const {
    return PI * (degrees / 180.0);
}

constexpr double CoordinateSystem::degrees(double radians) const {
    return 180.0 * (radians / PI);
}

}  // namespace common
