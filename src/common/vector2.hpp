#ifndef VECTOR_2_HPP
#define VECTOR_2_HPP

namespace common {

class Vector2 {
public:
    // Create from Cartesian coordinates
    Vector2(double x, double y);

    // Create from polar coordinates, angle in radians
    static Vector2 polar(double angle, double magnitude);

    double magnitude() const;
    double dot(const Vector2 rhs) const;

    Vector2& operator+=(const Vector2& rhs);
    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs);

    Vector2& operator-=(const Vector2& rhs);
    friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs);

    friend Vector2 operator*(const Vector2& lhs, double scalar);
    friend Vector2 operator*(double scalar, const Vector2& rhs);

    double x;
    double y;
};

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
Vector2 operator*(const Vector2& lhs, double scalar);
Vector2 operator*(double scalar, const Vector2& rhs);

}

#endif
