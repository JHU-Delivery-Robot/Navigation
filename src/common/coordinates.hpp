#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <iostream>

namespace common {

class Coordinates {
public:
    Coordinates(double latitude, double longitude);

    friend std::ostream& operator<<(std::ostream& output, const Coordinates& coordinates);

    double latitude;
    double longitude;
};

}  // namespace common

#endif
