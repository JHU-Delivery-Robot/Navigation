#include "coordinates.hpp"

#include <cmath>

namespace common {

Coordinates::Coordinates(double latitude, double longitude) : latitude(latitude), longitude(longitude) {}

Coordinates& Coordinates::operator+=(const Vector2& rhs) {
    latitude += rhs.y;
    longitude += rhs.x;

    return *this;
}

Coordinates operator+(const Coordinates& lhs, const Vector2& rhs) {
    return Coordinates(lhs.latitude + rhs.y, lhs.longitude + rhs.x);
}

Coordinates& Coordinates::operator-=(const Vector2& rhs) {
    latitude += rhs.y;
    longitude += rhs.x;

    return *this;
}

Coordinates operator-(const Coordinates& lhs, const Vector2& rhs) {
    return Coordinates(lhs.latitude - rhs.y, lhs.longitude - rhs.x);
}

Vector2 operator-(const Coordinates& lhs, const Coordinates& rhs) {
    return Vector2(lhs.longitude - rhs.longitude, lhs.latitude - rhs.latitude);
}

std::ostream& operator<<(std::ostream& output, const Coordinates& coordinates) {
    return output << "(" << coordinates.latitude << ", " << coordinates.longitude << ")";
}

}  // namespace common
