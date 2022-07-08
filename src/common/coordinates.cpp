#include "coordinates.hpp"

#include <cmath>

namespace common {

Coordinates::Coordinates(double latitude, double longitude) : latitude(latitude), longitude(longitude) {}

std::ostream& operator<<(std::ostream& output, const Coordinates& coordinates) {
    return output << "(" << coordinates.latitude << ", " << coordinates.longitude << ")";
}

}  // namespace common
