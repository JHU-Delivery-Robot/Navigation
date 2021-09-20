#include "vector2.hpp"

#include <cmath>

common::Vector2::Vector2(double x, double y) : x(x), y(y) { }

common::Vector2 common::Vector2::polar(double angle, double magnitude) {
    double x = magnitude*cos(angle);
    double y = magnitude*sin(angle);

    return Vector2(x, y);
}

double common::Vector2::magnitude() const {
    return sqrt(x*x + y*y);
}

double common::Vector2::dot(const common::Vector2 rhs) const {
    return x*rhs.x + y*rhs.y;
}

common::Vector2& common::Vector2::operator+=(const common::Vector2& rhs) {
    x += rhs.x;
    y += rhs.y;

    return *this;
}

common::Vector2 common::operator+(const common::Vector2& lhs, const common::Vector2& rhs) {
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

common::Vector2& common::Vector2::operator-=(const common::Vector2& rhs) {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
}

common::Vector2 common::operator-(const common::Vector2& lhs, const common::Vector2& rhs) {
    return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

common::Vector2 common::operator*(const common::Vector2& lhs, double scalar) {
    return Vector2(lhs.x * scalar, lhs.y * scalar);
}

common::Vector2 common::operator*(double scalar, const common::Vector2& rhs) {
    return Vector2(rhs.x * scalar, rhs.y * scalar);
}
