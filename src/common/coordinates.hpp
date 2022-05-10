#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <iostream>

#include "vector2.hpp"

namespace common {

class Coordinates {
public:
    Coordinates(double latitude, double longitude);

    Coordinates& operator+=(const Vector2& rhs);
    friend Coordinates operator+(const Coordinates& lhs, const Vector2& rhs);

    Coordinates& operator-=(const Vector2& rhs);
    friend Coordinates operator-(const Coordinates& lhs, const Vector2& rhs);

    friend Vector2 operator-(const Coordinates& lhs, const Coordinates& rhs);

    friend std::ostream& operator<<(std::ostream& output, const Coordinates& vector);

    double latitude;
    double longitude;
};

Coordinates operator+(const Coordinates& lhs, const Vector2& rhs);
Coordinates operator-(const Coordinates& lhs, const Vector2& rhs);
Vector2 operator-(const Coordinates& lhs, const Coordinates& rhs);

}

#endif
