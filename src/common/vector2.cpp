#include "vector2.hpp"

#include <cmath>

namespace common {

Vector2::Vector2(double x, double y) : x(x), y(y) { }

Vector2 Vector2::polar(double angle, double magnitude) {
    double x = magnitude*cos(angle);
    double y = magnitude*sin(angle);

    return Vector2(x, y);
}

double Vector2::magnitude() const {
    return sqrt(x*x + y*y);
}

double Vector2::dot(const Vector2 rhs) const {
    return x*rhs.x + y*rhs.y;
}

Vector2& Vector2::operator+=(const Vector2& rhs) {
    x += rhs.x;
    y += rhs.y;

    return *this;
}

Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vector2& Vector2::operator-=(const Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vector2 operator*(const Vector2& lhs, double scalar) {
    return Vector2(lhs.x * scalar, lhs.y * scalar);
}

Vector2 operator*(double scalar, const Vector2& rhs) {
    return Vector2(rhs.x * scalar, rhs.y * scalar);
}

}
